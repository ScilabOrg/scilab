/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>

#include "dynlib_scicos_blocks.h"
#include "scoUtils.h"

#include "MALLOC.h"
#include "elementary_functions.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "createGraphicObject.h"

#include "CurrentFigure.h"
#include "CurrentObject.h"

#include "scicos_block4.h"
#include "scicos.h"

#include "localization.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif

#include "FigureList.h"
#include "BuildObjects.h"
#include "AxesModel.h"

/*****************************************************************************
 * Internal container structure
 ****************************************************************************/

/**
 * Container structure
 */
typedef struct
{
    struct
    {
        int numberOfPoints;
        int maxNumberOfPoints;
        double *time;
        double ***data;
    } internal;

    struct
    {
        int periodCounter;

        char const* cachedFigureUID;
        char *cachedAxeUID;
        char **cachedPolylinesUIDs;
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
 * \param data the data to append
 */
static void appendData(scicos_block * block, int input, double t, double *data);

/**
 * Push the block data to the polyline
 *
 * \param block the block
 * \param input the selected input
 * \param row the selected row
 * \param pPolylineUID the polyline uid
 *
 */
static BOOL pushData(scicos_block * block, int input, int row);

/*****************************************************************************
 * Graphics utils
 ****************************************************************************/

/**
 * Get (and allocate on demand) the figure associated with the block
 * \param block the block
 * \return a valid figure UID or NULL on error
 */
static char const* getFigure(scicos_block * block);

/**
 * Get (and allocate on demand) the axe associated with the input
 *
 * \param pFigureUID the parent figure UID
 * \param block the block
 * \param input the current input index (0-indexed)
 * \return a valid axe UID or NULL on error
 */
static char *getAxe(char const* pFigureUID, scicos_block * block, int input);

/**
 * Get (and allocate on demand) the polyline associated with the row
 *
 * \param pAxeUID the parent axe UID
 * \param block the block
 * \param row the current row index (0-indexed)
 * \return a valid polyline UID or NULL on error
 */
static char *getPolyline(char *pAxeUID, scicos_block * block, int row);

/**
 * Set the polylines buffer size
 *
 * \param block the block
 * \param input the input port index
 * \param maxNumberOfPoints the size of the buffer
 */
static BOOL setPolylinesBuffers(scicos_block * block, int input, int maxNumberOfPoints);

/**
 * Set the polylines bounds
 *
 * \param block the block
 * \param input the input port index
 * \param periodCounter number of past periods since startup
 */
static BOOL setPolylinesBounds(scicos_block * block, int input, int periodCounter);

/*****************************************************************************
 * Simulation function
 ****************************************************************************/

/** \fn void cscope(scicos_block * block,int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An int which indicates the state of the block (init, update, ending)
*/
SCICOS_BLOCKS_IMPEXP void cscope(scicos_block * block, scicos_flag flag)
{
    char const* pFigureUID;

    double t;
    double *u;
    sco_data *sco;

    int i;
    BOOL result;

    switch (flag)
    {

        case Initialization:
            sco = getScoData(block);
            if (sco == NULL)
            {
                set_block_error(-5);
                break;
            }
            pFigureUID = getFigure(block);
            if (pFigureUID == NULL)
            {
                // allocation error
                set_block_error(-5);
                break;
            }
            break;

        case StateUpdate:
            pFigureUID = getFigure(block);
            if (pFigureUID == NULL)
            {
                // allocation error
                set_block_error(-5);
                break;
            }

            t = get_scicos_time();
            u = GetRealInPortPtrs(block, 1);

            appendData(block, 0, t, u);

            for (i = 0; i < block->insz[0]; i++)
            {
                result = pushData(block, 0, i);
                if (result == FALSE)
                {
                    Coserror("%s: unable to push some data.", "cscope");
                    break;
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
    sco_data *sco = (sco_data *) * (block->work);
    int i, j, k, l;

    if (sco == NULL)
    {
        /*
         * Data allocation
         */

        sco = (sco_data *) MALLOC(sizeof(sco_data));
        if (sco == NULL)
            goto error_handler_sco;

        sco->internal.numberOfPoints = 0;
        sco->internal.maxNumberOfPoints = block->ipar[2];

        sco->internal.data = (double ***)CALLOC(block->nin, sizeof(double **));
        if (sco->internal.data == NULL)
            goto error_handler_data;

        for (i = 0; i < block->nin; i++)
        {
            sco->internal.data[i] = (double **)CALLOC(block->insz[i], sizeof(double *));
            if (sco->internal.data[i] == NULL)
                goto error_handler_data_i;
        }
        for (i = 0; i < block->nin; i++)
        {
            for (j = 0; j < block->insz[i]; j++)
            {
                sco->internal.data[i][j] = (double *)CALLOC(block->ipar[2], sizeof(double));

                if (sco->internal.data[i][j] == NULL)
                    goto error_handler_data_ij;
            }
        }

        sco->internal.time = (double *)CALLOC(block->ipar[2], sizeof(double));
        if (sco->internal.time == NULL)
        {
            goto error_handler_time;
        }

        sco->scope.periodCounter = 0;
        sco->scope.cachedFigureUID = NULL;
        sco->scope.cachedAxeUID = NULL;
        sco->scope.cachedPolylinesUIDs = (char **)CALLOC(block->insz[0], sizeof(char *));

        *(block->work) = sco;
    }

    return sco;

    /*
     * Error management (out of normal flow)
     */

error_handler_time:
error_handler_data_ij:
    for (k = 0; k < i; k++)
    {
        for (l = 0; l < j; l++)
        {
            FREE(sco->internal.data[k][l]);
        }
    }
    i = block->nin - 1;
error_handler_data_i:
    for (j = 0; j < i; j++)
    {
        FREE(sco->internal.data[i]);
    }
    FREE(sco->internal.data);
error_handler_data:
    FREE(sco);
error_handler_sco:
    // allocation error
    set_block_error(-5);
    return NULL;
}

static void freeScoData(scicos_block * block)
{
    sco_data *sco = (sco_data *) * (block->work);
    int i, j;

    if (sco != NULL)
    {
        for (i = 0; i < block->nin; i++)
        {
            for (j = 0; j < block->insz[i]; j++)
            {
                FREE(sco->internal.data[i][j]);
            }
            FREE(sco->internal.data[i]);
        }

        FREE(sco->internal.data);
        FREE(sco->internal.time);

        for (i = 0; i < block->insz[0]; i++)
        {
            FREE(sco->scope.cachedPolylinesUIDs[i]);
        }
        FREE(sco->scope.cachedAxeUID);

        FREE(sco);
    }
}

static sco_data *reallocScoData(scicos_block * block, int numberOfPoints)
{
    sco_data *sco = (sco_data *) * (block->work);
    int i, j;

    double *ptr;
    int setLen;
    int previousNumberOfPoints = sco->internal.maxNumberOfPoints;

    for (i = 0; i < block->nin; i++)
    {
        for (j = 0; j < block->insz[i]; j++)
        {
            ptr = (double *)REALLOC(sco->internal.data[i][j], numberOfPoints * sizeof(double));
            if (ptr == NULL)
                goto error_handler;

            for (setLen = numberOfPoints - previousNumberOfPoints - 1; setLen >= 0; setLen--)
                ptr[previousNumberOfPoints + setLen] = ptr[previousNumberOfPoints - 1];
            sco->internal.data[i][j] = ptr;
        }
    }

    ptr = (double *)REALLOC(sco->internal.time, numberOfPoints * sizeof(double));
    if (ptr == NULL)
        goto error_handler;

    for (setLen = numberOfPoints - previousNumberOfPoints - 1; setLen >= 0; setLen--)
        ptr[previousNumberOfPoints + setLen] = ptr[previousNumberOfPoints - 1];
    sco->internal.time = ptr;

    sco->internal.maxNumberOfPoints = numberOfPoints;
    return sco;

error_handler:
    freeScoData(block);
    // allocation error
    set_block_error(-5);
    return NULL;
}

static void appendData(scicos_block * block, int input, double t, double *data)
{
    int i;

    sco_data *sco = (sco_data *) * (block->work);
    int maxNumberOfPoints = sco->internal.maxNumberOfPoints;
    int numberOfPoints = sco->internal.numberOfPoints;

    /*
     * Handle the case where the t is greater than the data_bounds
     */
    if (t > ((sco->scope.periodCounter + 1) * block->rpar[3]))
    {
        sco->scope.periodCounter++;

        numberOfPoints = 0;
        sco->internal.numberOfPoints = 0;
        if (setPolylinesBounds(block, input, sco->scope.periodCounter) == FALSE)
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
        maxNumberOfPoints = maxNumberOfPoints + block->ipar[2];
        sco = reallocScoData(block, maxNumberOfPoints);

        // reconfigure related graphic objects
        if (setPolylinesBuffers(block, input, maxNumberOfPoints) == FALSE)
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
        int setLen;

        for (i = 0; i < block->insz[input]; i++)
        {
            for (setLen = maxNumberOfPoints - numberOfPoints - 1; setLen >= 0; setLen--)
                sco->internal.data[input][i][numberOfPoints + setLen] = data[i];
        }

        for (setLen = maxNumberOfPoints - numberOfPoints - 1; setLen >= 0; setLen--)
            sco->internal.time[numberOfPoints + setLen] = t;

        sco->internal.numberOfPoints++;
    }
}

static BOOL pushData(scicos_block * block, int input, int row)
{
    char const* pFigureUID;
    char *pAxeUID;
    char *pPolylineUID;

    double *data;
    sco_data *sco;

    BOOL result = TRUE;

    pFigureUID = getFigure(block);
    pAxeUID = getAxe(pFigureUID, block, input);
    pPolylineUID = getPolyline(pAxeUID, block, row);

    sco = getScoData(block);
    if (sco == NULL)
        return FALSE;

    // select the right input and row
    data = sco->internal.data[input][row];

    result &= setGraphicObjectProperty(pPolylineUID, __GO_DATA_MODEL_X__, sco->internal.time, jni_double_vector, sco->internal.maxNumberOfPoints);
    result &= setGraphicObjectProperty(pPolylineUID, __GO_DATA_MODEL_Y__, data, jni_double_vector, sco->internal.maxNumberOfPoints);

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
static void setFigureSettings(char const* pFigureUID, scicos_block * block)
{
    int nipar = GetNipar(block);
    int *ipar = GetIparPtrs(block);

    int win_pos[2];
    int win_dim[2];

    win_pos[0] = ipar[(nipar - 1) - 3];
    win_pos[1] = ipar[(nipar - 1) - 2];
    win_dim[0] = ipar[(nipar - 1) - 1];
    win_dim[1] = ipar[nipar - 1];

    if (win_pos[0] > 0 && win_pos[1] > 0)
    {
        setGraphicObjectProperty(pFigureUID, __GO_POSITION__, &win_pos, jni_int_vector, 2);
    }

    if (win_dim[0] > 0 && win_dim[1] > 0)
    {
        setGraphicObjectProperty(pFigureUID, __GO_SIZE__, &win_dim, jni_int_vector, 2);
    }
};

/*****************************************************************************
 *
 * Graphic
 *
 ****************************************************************************/

static char const* getFigure(scicos_block * block)
{
    signed int figNum;
    char const* pFigureUID = NULL;
    char *pAxe = NULL;
    int i__1 = 1;
    sco_data *sco = (sco_data *) * (block->work);

    int i;

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

        // the stored uid is a reference to the figure map, not to the current figure
        pFigureUID = getFigureFromIndex(figNum);
        sco->scope.cachedFigureUID = pFigureUID;

        // set configured parameters
        setFigureSettings(pFigureUID, block);

        // allocate the axes through the getter
        for (i = 0; i < GetNin(block); i++)
        {
            pAxe = getAxe(pFigureUID, block, i);

            /*
             * Setup according to block settings
             */
            setLabel(pAxe, __GO_X_AXIS_LABEL__, "t");
            setLabel(pAxe, __GO_Y_AXIS_LABEL__, "y");

            setGraphicObjectProperty(pAxe, __GO_X_AXIS_VISIBLE__, &i__1, jni_bool, 1);
            setGraphicObjectProperty(pAxe, __GO_Y_AXIS_VISIBLE__, &i__1, jni_bool, 1);

            setPolylinesBounds(block, i, 0);
        }
    }

    if (sco->scope.cachedFigureUID == NULL)
    {
        sco->scope.cachedFigureUID = pFigureUID;
    }
    return pFigureUID;
}

static char *getAxe(char const* pFigureUID, scicos_block * block, int input)
{
    char *pAxe;
    int i;
    sco_data *sco = (sco_data *) * (block->work);

    // fast path for an existing object
    if (sco->scope.cachedAxeUID != NULL)
    {
        return sco->scope.cachedAxeUID;
    }

    pAxe = findChildWithKindAt(pFigureUID, __GO_AXES__, input);

    /*
     * Allocate if necessary
     */
    if (pAxe == NULL)
    {
        cloneAxesModel(pFigureUID);
        pAxe = findChildWithKindAt(pFigureUID, __GO_AXES__, input);
    }

    /*
     * Setup on first access
     */
    if (pAxe != NULL)
    {
        // allocate the polylines through the getter
        for (i = 0; i < block->insz[input]; i++)
        {
            getPolyline(pAxe, block, i);
        }
    }

    /*
     * then cache with local storage
     */
    if (pAxe != NULL && sco->scope.cachedAxeUID == NULL)
    {
        sco->scope.cachedAxeUID = strdup(pAxe);
        releaseGraphicObjectProperty(__GO_PARENT__, pAxe, jni_string, 1);
    }
    return sco->scope.cachedAxeUID;
}

static char *getPolyline(char *pAxeUID, scicos_block * block, int row)
{
    char *pPolyline;
    double d__0 = 0.0;
    BOOL b__true = TRUE;

    int color;

    sco_data *sco = (sco_data *) * (block->work);

    // fast path for an existing object
    if (sco->scope.cachedPolylinesUIDs != NULL && sco->scope.cachedPolylinesUIDs[row] != NULL)
    {
        return sco->scope.cachedPolylinesUIDs[row];
    }

    pPolyline = findChildWithKindAt(pAxeUID, __GO_POLYLINE__, row);

    /*
     * Allocate if necessary
     */
    if (pPolyline == NULL)
    {
        pPolyline = createGraphicObject(__GO_POLYLINE__);

        if (pPolyline != NULL)
        {
            createDataObject(pPolyline, __GO_POLYLINE__);
            setGraphicObjectRelationship(pAxeUID, pPolyline);
        }
    }

    /*
     * Setup on first access
     */
    if (pPolyline != NULL)
    {

        /*
         * Default setup (will crash if removed)
         */
        {
            int polylineSize[2] = { 1, block->ipar[2] };
            setGraphicObjectProperty(pPolyline, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, polylineSize, jni_int_vector, 2);
        }

        setGraphicObjectProperty(pPolyline, __GO_DATA_MODEL_X__, &d__0, jni_double_vector, 1);
        setGraphicObjectProperty(pPolyline, __GO_DATA_MODEL_Y__, &d__0, jni_double_vector, 1);

        color = block->ipar[3 + row];
        if (color > 0)
        {
            setGraphicObjectProperty(pPolyline, __GO_LINE_MODE__, &b__true, jni_bool, 1);
            setGraphicObjectProperty(pPolyline, __GO_LINE_COLOR__, &color, jni_int, 1);
        }
        else
        {
            color = -color;
            setGraphicObjectProperty(pPolyline, __GO_MARK_MODE__, &b__true, jni_bool, 1);
            setGraphicObjectProperty(pPolyline, __GO_MARK_STYLE__, &color, jni_int, 1);
        }

        {
            int iClipState = 1; //on
            setGraphicObjectProperty(pPolyline, __GO_CLIP_STATE__, &iClipState, jni_int, 1);
        }
    }

    /*
     * then cache with local storage
     */
    if (pPolyline != NULL && sco->scope.cachedPolylinesUIDs != NULL && sco->scope.cachedPolylinesUIDs[row] == NULL)
    {
        sco->scope.cachedPolylinesUIDs[row] = strdup(pPolyline);
        releaseGraphicObjectProperty(__GO_PARENT__, pPolyline, jni_string, 1);
    }
    return sco->scope.cachedPolylinesUIDs[row];
}

static BOOL setPolylinesBuffers(scicos_block * block, int input, int maxNumberOfPoints)
{
    int i;

    char const* pFigureUID;
    char *pAxeUID;
    char *pPolylineUID;

    BOOL result = TRUE;
    int polylineSize[2] = { 1, maxNumberOfPoints };

    pFigureUID = getFigure(block);
    pAxeUID = getAxe(pFigureUID, block, input);

    for (i = 0; i < block->insz[input]; i++)
    {
        pPolylineUID = getPolyline(pAxeUID, block, i);
        result &= setGraphicObjectProperty(pPolylineUID, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, polylineSize, jni_int_vector, 2);
    }

    return result;
}

static BOOL setPolylinesBounds(scicos_block * block, int input, int periodCounter)
{
    char const* pFigureUID;
    char *pAxeUID;

    double dataBounds[6];
    double period = block->rpar[3];

    dataBounds[0] = periodCounter * period; // xMin
    dataBounds[1] = (periodCounter + 1) * period;   // xMax
    dataBounds[2] = block->rpar[1]; // yMin
    dataBounds[3] = block->rpar[2]; // yMax
    dataBounds[4] = -1.0;       // zMin
    dataBounds[5] = 1.0;        // zMax

    pFigureUID = getFigure(block);
    pAxeUID = getAxe(pFigureUID, block, input);
    return setGraphicObjectProperty(pAxeUID, __GO_DATA_BOUNDS__, dataBounds, jni_double_vector, 6);
}
