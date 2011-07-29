/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Clément DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "dynlib_scicos_blocks.h"
#include "scoUtils.h"

#include "MALLOC.h"
#include "elementary_functions.h"
#include "string.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "createGraphicObject.h"

#include "CurrentFigure.h"

#include "scicos_block4.h"
#include "scicos.h"

#include "localization.h"

/*****************************************************************************
 * Internal container structure
 ****************************************************************************/

#define DEFAULT_MAX_NUMBER_OF_POINTS 8

/**
 * Container structure
 */
typedef struct
{
    struct
    {
        int *numberOfPoints;
        int *maxNumberOfPoints;
        double **data;
    } internal;

    struct
    {
        int periodCounter;

        char *cachedFigureUID;
        char *cachedAxeUID;
        char **cachedSegsUIDs;
    } scope;
} sco_data;

/**
 * Get (and allocate on demand) the internal data used on this scope
 * \param block the block
 * \return the scope data
 */
static sco_data *getScoData(scicos_block * block);

/**
 * Release any internal data
 *
 * \param block the block
 */
static void freeScoData(scicos_block * block);

/**
 * Append the data to the current data
 *
 * \param block the block
 * \param input the input (0-indexed)
 * \param t the current time
 */
static void appendData(scicos_block * block, int input, double t);

/**
 * Push the block data to the segs
 *
 * \param block the block
 * \param input the selected input
 *
 */
static BOOL pushData(scicos_block * block, int input);

/*****************************************************************************
 * Graphics utils
 ****************************************************************************/

/**
 * Get (and allocate on demand) the figure associated with the block
 * \param block the block
 * \return a valid figure UID or NULL on error
 */
static char *getFigure(scicos_block * block);

/**
 * Get (and allocate on demand) the axe associated with the input
 *
 * \param pFigureUID the parent figure UID
 * \param block the block
 * \return a valid axe UID or NULL on error
 */
static char *getAxe(char *pFigureUID, scicos_block * block);

/**
 * Get (and allocate on demand) the segs associated with the input
 *
 * \param pAxeUID the parent axe UID
 * \param block the block
 * \param input the current row index (0-indexed)
 * \return a valid polyline UID or NULL on error
 */
static char *getSegs(char *pAxeUID, scicos_block * block, int input);

/**
 * Set the segs buffer size
 *
 * \param block the block
 * \param input the input port index
 * \param maxNumberOfPoints the size of the buffer
 */
static BOOL setSegsBuffers(scicos_block * block, int input, int maxNumberOfPoints);

/**
 * Set the axes bounds
 *
 * \param block the block
 * \param periodCounter number of past periods since startup
 */
static BOOL setBounds(scicos_block * block, int periodCounter);

/*****************************************************************************
 * Simulation function
 ****************************************************************************/

/** \fn void cscope(scicos_block * block,int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An int which indicates the state of the block (init, update, ending)
*/
SCICOS_BLOCKS_IMPEXP void cevscpe(scicos_block * block, scicos_flag flag)
{
    char *pFigureUID;

    double t;
    int i;
    int mask;
    int nclk = block->nipar - 6;
    sco_data *sco;

    BOOL result;

    switch (flag)
    {

    case Initialization:
        sco = getScoData(block);
        if (sco == NULL)
        {
            set_block_error(-5);
        }
        pFigureUID = getFigure(block);
        if (pFigureUID == NULL)
        {
            // allocation error
            set_block_error(-5);
        }
        break;

    case StateUpdate:
        pFigureUID = getFigure(block);

        t = get_scicos_time();

        // select only the masked indexes
        for (i = 0; i < nclk; i++)
        {
            mask = 1 << i;
            if ((block->nevprt & mask) == mask)
            {
                appendData(block, i, t);

                result = pushData(block, i);
                if (result == FALSE)
                {
                    Coserror("%s: unable to push some data.", "cevscpe");
                    break;
                }
            }
        }
        break;

    case Ending:
        freeScoData(block);
        break;

    default:
        break;
    }
}

/*-------------------------------------------------------------------------*/

/*****************************************************************************
 *
 * Container management
 *
 ****************************************************************************/

static sco_data *getScoData(scicos_block * block)
{
    static const int i__1;

    sco_data *sco = (sco_data *) * (block->work);
    int i, j, k, l;
    BOOL result;

    int nclk = block->nipar - 6;

    if (sco == NULL)
    {
        /*
         * Data allocation
         */

        sco = (sco_data *) MALLOC(sizeof(sco_data));
        if (sco == NULL)
            goto error_handler_sco;

        sco->internal.numberOfPoints = (int *)CALLOC(nclk, sizeof(int));
        if (sco->internal.numberOfPoints == NULL)
        {
            goto error_handler_numberOfPoints;
        }
        sco->internal.maxNumberOfPoints = (int *)MALLOC(nclk * sizeof(int));
        if (sco->internal.numberOfPoints == NULL)
        {
            goto error_handler_maxNumberOfPoints;
        }
        for (i = 0; i < nclk; i++)
        {
            sco->internal.maxNumberOfPoints[i] = DEFAULT_MAX_NUMBER_OF_POINTS;
        }

        sco->internal.data = (double **)CALLOC(2 * nclk, sizeof(double *));
        if (sco->internal.data == NULL)
            goto error_handler_data;

        for (i = 0; i < nclk; i++)
        {
            /*
             * Alloc base pointer
             */
            sco->internal.data[2 * i + 0] = (double *)CALLOC(3 * DEFAULT_MAX_NUMBER_OF_POINTS, sizeof(double));
            if (sco->internal.data[2 * i + 0] == NULL)
                goto error_handler_data_i;

            /*
             * Alloc direction pointer
             */
            sco->internal.data[2 * i + 1] = (double *)CALLOC(3 * DEFAULT_MAX_NUMBER_OF_POINTS, sizeof(double));
            if (sco->internal.data[2 * i + 1] == NULL)
            {
                FREE(sco->internal.data[2 * i + 0]);
                goto error_handler_data_i;
            }
        }

        sco->scope.periodCounter = 0;
        sco->scope.cachedFigureUID = NULL;
        sco->scope.cachedAxeUID = NULL;
        sco->scope.cachedSegsUIDs = (char **)CALLOC(nclk, sizeof(char *));

        *(block->work) = sco;
    }

    return sco;

    /*
     * Error management (out of normal flow)
     */

error_handler_data_i:
    for (j = 0; j < i; j++)
    {
        FREE(sco->internal.data[2 * j + 0]);
        FREE(sco->internal.data[2 * j + 1]);
    }
    FREE(sco->internal.data);
error_handler_data:
    FREE(sco->internal.maxNumberOfPoints);
error_handler_maxNumberOfPoints:
    FREE(sco->internal.numberOfPoints);
error_handler_numberOfPoints:
    FREE(sco);
error_handler_sco:
    // allocation error
    set_block_error(-5);
    return NULL;
}

static void freeScoData(scicos_block * block)
{
    sco_data *sco = getScoData(block);
    int i, j;

    int nclk = block->nipar - 6;

    if (sco != NULL)
    {
        for (i = 0; i < nclk; i++)
        {
            FREE(sco->internal.data[i]);
        }
        FREE(sco->internal.data);
        FREE(sco->internal.maxNumberOfPoints);
        FREE(sco->internal.numberOfPoints);

//      Commented due to the C++ allocation
//      see http://bugzilla.scilab.org/show_bug.cgi?id=9747
//      FREE(sco->scope.cachedFigureUID);
//      FREE(sco->scope.cachedAxeUID);
//      sco->scope.cachedFigureUID = NULL;
//      for (i=0; i<nclk; i++) {
//              FREE(sco->scope.cachedPolylinesUIDs[i]);
//              sco->scope.cachedPolylinesUIDs[i] = NULL;
//
//
//
//      }
//      sco->scope.cachedAxeUID = NULL;

        FREE(sco);
    }
}

static sco_data *reallocScoData(scicos_block * block, int input, int numberOfPoints)
{
    static const int i__3 = 3;

    sco_data *sco = getScoData(block);
    int nclk = block->nipar - 6;
    double *ptr;
    int maxNumberOfPoints = sco->internal.maxNumberOfPoints[input];
    int setLen;

    setLen = numberOfPoints - maxNumberOfPoints;

    /*
     * Realloc base pointer
     */
    ptr = (double *)REALLOC(sco->internal.data[2 * input], 3 * numberOfPoints * sizeof(double));
    if (ptr == NULL)
        goto error_handler;

    C2F(dset) (&setLen, &ptr[3 * (maxNumberOfPoints - 1) + 0], &(ptr[3 * maxNumberOfPoints + 0]), &i__3);
    C2F(dset) (&setLen, &ptr[3 * (maxNumberOfPoints - 1) + 1], &(ptr[3 * maxNumberOfPoints + 1]), &i__3);
    C2F(dset) (&setLen, &ptr[3 * (maxNumberOfPoints - 1) + 2], &(ptr[3 * maxNumberOfPoints + 2]), &i__3);
    sco->internal.data[2 * input] = ptr;

    /*
     * Realloc direction pointer
     */
    ptr = (double *)REALLOC(sco->internal.data[2 * input + 1], 3 * numberOfPoints * sizeof(double));
    if (ptr == NULL)
        goto error_handler;

    C2F(dset) (&setLen, &ptr[3 * (maxNumberOfPoints - 1) + 0], &(ptr[3 * maxNumberOfPoints + 0]), &i__3);
    C2F(dset) (&setLen, &ptr[3 * (maxNumberOfPoints - 1) + 1], &(ptr[3 * maxNumberOfPoints + 1]), &i__3);
    C2F(dset) (&setLen, &ptr[3 * (maxNumberOfPoints - 1) + 2], &(ptr[3 * maxNumberOfPoints + 2]), &i__3);
    sco->internal.data[2 * input + 1] = ptr;

    sco->internal.maxNumberOfPoints[input] = numberOfPoints;
    return sco;

error_handler:
    freeScoData(block);
    // allocation error
    set_block_error(-5);
    return NULL;
}

static void appendData(scicos_block * block, int input, double t)
{
    int i;
    static const int i__3 = 3;

    sco_data *sco = getScoData(block);
    int maxNumberOfPoints = sco->internal.maxNumberOfPoints[input];
    int numberOfPoints = sco->internal.numberOfPoints[input];

    /*
     * Handle the case where the t is greater than the data_bounds
     */
    if (t > ((sco->scope.periodCounter + 1) * block->rpar[0]))
    {
        sco->scope.periodCounter++;

        numberOfPoints = 0;
        sco->internal.numberOfPoints[input] = 0;
        if (setBounds(block, sco->scope.periodCounter) == FALSE)
        {
            set_block_error(-5);
            freeScoData(block);
            sco = NULL;
        }
    }

    /*
     * Handle the case where the scope has more points than maxNumberOfPoints
     */
    if (sco != NULL && numberOfPoints >= maxNumberOfPoints)
    {
        // on a full scope, re-alloc
        maxNumberOfPoints = maxNumberOfPoints + DEFAULT_MAX_NUMBER_OF_POINTS;
        sco = reallocScoData(block, input, maxNumberOfPoints);

        // reconfigure related graphic objects
        if (setSegsBuffers(block, input, maxNumberOfPoints) == FALSE)
        {
            set_block_error(-5);
            freeScoData(block);
            sco = NULL;
        }
    }

    /*
     * Update data
     */
    if (sco != NULL)
    {
        int setLen = maxNumberOfPoints - numberOfPoints - 1;

        /*
         * Base pointer
         */
        sco->internal.data[2 * input][3 * numberOfPoints + 0] = t;  // x
        sco->internal.data[2 * input][3 * numberOfPoints + 1] = 0;  // y
        sco->internal.data[2 * input][3 * numberOfPoints + 2] = (double)input;  // z

        C2F(dset) (&setLen, &sco->internal.data[2 * input][3 * numberOfPoints + 0], &(sco->internal.data[2 * input][3 * (numberOfPoints + 1) + 0]),
                   &i__3);
        C2F(dset) (&setLen, &sco->internal.data[2 * input][3 * numberOfPoints + 1], &(sco->internal.data[2 * input][3 * (numberOfPoints + 1) + 1]),
                   &i__3);
        C2F(dset) (&setLen, &sco->internal.data[2 * input][3 * numberOfPoints + 2], &(sco->internal.data[2 * input][3 * (numberOfPoints + 1) + 2]),
                   &i__3);

        /*
         * Direction pointer
         */
        sco->internal.data[2 * input + 1][3 * numberOfPoints + 0] = t;  // x
        sco->internal.data[2 * input + 1][3 * numberOfPoints + 1] = 0.8;    // y
        sco->internal.data[2 * input + 1][3 * numberOfPoints + 2] = (double)input;  // z

        C2F(dset) (&setLen, &sco->internal.data[2 * input + 1][3 * numberOfPoints + 0],
                   &(sco->internal.data[2 * input + 1][3 * (numberOfPoints + 1) + 0]), &i__3);
        C2F(dset) (&setLen, &sco->internal.data[2 * input + 1][3 * numberOfPoints + 1],
                   &(sco->internal.data[2 * input + 1][3 * (numberOfPoints + 1) + 1]), &i__3);
        C2F(dset) (&setLen, &sco->internal.data[2 * input + 1][3 * numberOfPoints + 2],
                   &(sco->internal.data[2 * input + 1][3 * (numberOfPoints + 1) + 2]), &i__3);

        sco->internal.numberOfPoints[input]++;
    }
}

static BOOL pushData(scicos_block * block, int input)
{
    char *pFigureUID;
    char *pAxeUID;
    char *pSegsUID;

    static const int i__0 = 0;

    int dataLen;
    double *base;
    double *direction;
    sco_data *sco;

    BOOL result = TRUE;

    pFigureUID = getFigure(block);
    pAxeUID = getAxe(pFigureUID, block);
    pSegsUID = getSegs(pAxeUID, block, input);

    sco = getScoData(block);
    if (sco == NULL)
        return FALSE;

    // select the right input and row
    base = sco->internal.data[2 * input];
    direction = sco->internal.data[2 * input + 1];

    dataLen = 3 * sco->internal.maxNumberOfPoints[input];

    result &= setGraphicObjectProperty(pSegsUID, __GO_BASE__, base, jni_double_vector, dataLen);
    result &= setGraphicObjectProperty(pSegsUID, __GO_DIRECTION__, direction, jni_double_vector, dataLen);

    return result;
}

/*****************************************************************************
 *
 * Graphic utils
 *
 ****************************************************************************/

/**
 * Set properties on the figure.
 *
 * \param pFigureUID the figure uid
 * \param block the current block
 */
static void setFigureSettings(char *pFigureUID, scicos_block * block)
{
    int nipar = GetNipar(block);
    int *ipar = GetIparPtrs(block);

    int win_pos[2];
    int win_dim[2];

    win_pos[0] = ipar[(nipar - 1) - 3];
    win_pos[1] = ipar[(nipar - 1) - 2];
    win_dim[0] = ipar[(nipar - 1) - 1];
    win_dim[1] = ipar[nipar - 1];

    setGraphicObjectProperty(pFigureUID, __GO_POSITION__, &win_pos, jni_int_vector, 2);
    setGraphicObjectProperty(pFigureUID, __GO_SIZE__, &win_dim, jni_int_vector, 2);
};

/*****************************************************************************
 *
 * Graphic
 *
 ****************************************************************************/

static char *getFigure(scicos_block * block)
{
    signed int figNum;
    char *pFigureUID = NULL;
    char *pAxe = NULL;
    int nclk = block->nipar - 6;
    sco_data *sco = getScoData(block);

    int i, j;

    // fast path for an existing object
    if (sco->scope.cachedFigureUID != NULL)
    {
        return sco->scope.cachedFigureUID;
    }

    figNum = block->ipar[0];

    // with a negative id, use the block number indexed from a constant.
    if (figNum < 0)
    {
        figNum = 20000 + get_block_number();
    }

    pFigureUID = getFigureFromIndex(figNum);
    // create on demand
    if (pFigureUID == NULL)
    {
        pFigureUID = createNewFigureWithAxes();
        setGraphicObjectProperty(pFigureUID, __GO_ID__, &figNum, jni_int, 1);

        // set configured parameters
        setFigureSettings(pFigureUID, block);

        // allocate the axes through the getter
        pAxe = getAxe(pFigureUID, block);

        /*
         * Setup according to block settings
         */
        setLabel(pAxe, __GO_X_AXIS_LABEL__, "t");
        setLabel(pAxe, __GO_Y_AXIS_LABEL__, "y");

        setBounds(block, 0);

        sco->scope.cachedFigureUID = pFigureUID;
    }

    if (sco->scope.cachedFigureUID == NULL)
    {
        sco->scope.cachedFigureUID = pFigureUID;
    }
    return pFigureUID;
}

static char *getAxe(char *pFigureUID, scicos_block * block)
{
    char *pAxe;
    int i;

    int nclk = block->nipar - 6;
    sco_data *sco = getScoData(block);

    // fast path for an existing object
    if (sco->scope.cachedAxeUID != NULL)
    {
        return sco->scope.cachedAxeUID;
    }

    pAxe = findChildWithKindAt(pFigureUID, __GO_AXES__, 0);

    /*
     * Allocate if necessary
     */
    if (pAxe == NULL)
    {
        pAxe = createGraphicObject(__GO_AXES__);

        if (pAxe != NULL)
        {
            setGraphicObjectRelationship(pFigureUID, pAxe);

            // allocate the polylines through the getter
            for (i = 0; i < nclk; i++)
            {
                getSegs(pAxe, block, i);
            }

            sco->scope.cachedAxeUID = pAxe;
        }
    }

    if (sco->scope.cachedAxeUID == NULL)
    {
        sco->scope.cachedAxeUID = pAxe;
    }
    return pAxe;
}

static char *getSegs(char *pAxeUID, scicos_block * block, int input)
{
    char *pSegs;

    sco_data *sco = getScoData(block);

    // fast path for an existing object
    if (sco->scope.cachedSegsUIDs != NULL && sco->scope.cachedSegsUIDs[input] != NULL)
    {
        return sco->scope.cachedSegsUIDs[input];
    }

    pSegs = findChildWithKindAt(pAxeUID, __GO_SEGS__, input);

    /*
     * Allocate if necessary
     */
    if (pSegs == NULL)
    {
        pSegs = createGraphicObject(__GO_SEGS__);

        if (pSegs != NULL)
        {
            createDataObject(pSegs, __GO_SEGS__);
            setGraphicObjectRelationship(pAxeUID, pSegs);

            setGraphicObjectProperty(pSegs, __GO_NUMBER_ARROWS__, &sco->internal.maxNumberOfPoints[input], jni_int, 1);

            sco->scope.cachedSegsUIDs[input] = pSegs;
        }
    }

    if (sco->scope.cachedSegsUIDs != NULL && sco->scope.cachedSegsUIDs[input] == NULL)
    {
        sco->scope.cachedSegsUIDs[input] = pSegs;
    }
    return pSegs;
}

static BOOL setSegsBuffers(scicos_block * block, int input, int maxNumberOfPoints)
{
    char *pFigureUID;
    char *pAxeUID;
    char *pSegsUID;

    int i;
    int nclk = block->nipar - 6;
    BOOL result = TRUE;

    pFigureUID = getFigure(block);
    pAxeUID = getAxe(pFigureUID, block);
    for (i = 0; i < nclk; i++)
    {
        pSegsUID = getSegs(pAxeUID, block, i);
        result &= setGraphicObjectProperty(pSegsUID, __GO_NUMBER_ARROWS__, &maxNumberOfPoints, jni_int, 1);
    }

    return result;
}

static BOOL setBounds(scicos_block * block, int periodCounter)
{
    char *pFigureUID;
    char *pAxeUID;

    BOOL result;
    double dataBounds[6];
    double period = block->rpar[0];

    dataBounds[0] = periodCounter * period; // xMin
    dataBounds[1] = (periodCounter + 1) * period;   // xMax
    dataBounds[2] = 0;          // yMin
    dataBounds[3] = 1;          // yMax
    dataBounds[4] = -1.0;       // zMin
    dataBounds[5] = 1.0;        // zMax

    pFigureUID = getFigure(block);
    pAxeUID = getAxe(pFigureUID, block);
    return setGraphicObjectProperty(pAxeUID, __GO_DATA_BOUNDS__, dataBounds, jni_double_vector, 6);
}
