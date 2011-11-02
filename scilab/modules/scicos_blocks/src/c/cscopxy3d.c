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

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "createGraphicObject.h"

#include "CurrentFigure.h"

#include "scicos_block4.h"
#include "scicos.h"

#include "localization.h"

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
        double ***data;
    } internal;

    struct
    {
        char *cachedFigureUID;
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
 * \param x x data
 * \param y y data
 * \param z z data
 */
static void appendData(scicos_block * block, double *x, double *y, double *z);

/**
 * Push the block data to the polyline
 *
 * \param block the block
 * \param row the selected row
 *
 */
static BOOL pushData(scicos_block * block, int row);

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
 * \param input the current input index (0-indexed)
 * \return a valid axe UID or NULL on error
 */
static char *getAxe(char *pFigureUID, scicos_block * block);

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
 * \param maxNumberOfPoints the size of the buffer
 */
static BOOL setPolylinesBuffers(scicos_block * block, int maxNumberOfPoints);

/**
 * Set the polylines bounds
 *
 * \param block the block
 */
static BOOL setPolylinesBounds(scicos_block * block);

/*****************************************************************************
 * Simulation function
 ****************************************************************************/

/** \fn void cmscope(scicos_block * block,int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An int which indicates the state of the block (init, update, ending)
*/
SCICOS_BLOCKS_IMPEXP void cscopxy3d(scicos_block * block, scicos_flag flag)
{
    char *pFigureUID;

    double *u;
    sco_data *sco;

    int i, j;
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

        appendData(block, (double *)block->inptr[0], (double *)block->inptr[1], (double *)block->inptr[2]);
        for (j = 0; j < block->insz[0]; j++)
        {
            result = pushData(block, j);
            if (result == FALSE)
            {
                Coserror("%s: unable to push some data.", "cscopxy3d");
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
    BOOL result;

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

        sco->scope.cachedFigureUID = NULL;
        sco->scope.cachedAxeUID = NULL;

        sco->scope.cachedPolylinesUIDs = (char **)CALLOC(block->insz[0], sizeof(char **));

        *(block->work) = sco;
    }

    return sco;

    /*
     * Error management (out of normal flow)
     */

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

//      Commented due to the C++ allocation
//      see http://bugzilla.scilab.org/show_bug.cgi?id=9747
//      FREE(sco->scope.cachedFigureUID);
//      sco->scope.cachedFigureUID = NULL;
//      for (i=0; i<block->nin; i++) {
//          for (j=0; j<block->insz[i]; j++) {
//              FREE(sco->scope.cachedPolylinesUIDs[i][j]);
//              sco->scope.cachedPolylinesUIDs[i][j] = NULL;
//          }
//          FREE(sco->scope.cachedAxeUID[i]);
//          sco->scope.cachedAxeUID[i] = NULL;
//      }

        FREE(sco);
    }
}

static sco_data *reallocScoData(scicos_block * block, int numberOfPoints)
{
    static const int i__1 = 1;

    sco_data *sco = (sco_data *) * (block->work);
    int i, j;
    double *ptr;
    int setLen;

    setLen = sco->internal.maxNumberOfPoints - numberOfPoints;

    for (i = 0; i < block->nin; i++)
    {
        for (j = 0; j < block->insz[i]; j++)
        {
            ptr = (double *)REALLOC(sco->internal.data[i][j], numberOfPoints * sizeof(double));
            if (ptr == NULL)
                goto error_handler;

            C2F(dset) (&setLen, &ptr[sco->internal.maxNumberOfPoints - 1], &(ptr[sco->internal.maxNumberOfPoints]), &i__1);
            sco->internal.data[i][j] = ptr;
        }
    }

    sco->internal.maxNumberOfPoints = numberOfPoints;
    return sco;

error_handler:
    freeScoData(block);
    // allocation error
    set_block_error(-5);
    return NULL;
}

static void appendData(scicos_block * block, double *x, double *y, double *z)
{
    int i;
    static const int i__1 = 1;

    sco_data *sco = (sco_data *) * (block->work);
    int maxNumberOfPoints = sco->internal.maxNumberOfPoints;
    int numberOfPoints = sco->internal.numberOfPoints;

    /*
     * Handle the case where the scope has more points than maxNumberOfPoints
     */
    if (sco != NULL && numberOfPoints >= maxNumberOfPoints)
    {
        // on a full scope, re-alloc
        maxNumberOfPoints = maxNumberOfPoints + block->ipar[2];
        sco = reallocScoData(block, maxNumberOfPoints);

        // reconfigure related graphic objects
        if (setPolylinesBuffers(block, maxNumberOfPoints) == FALSE)
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

        for (i = 0; i < block->insz[0]; i++)
        {
            sco->internal.data[0][i][numberOfPoints] = x[i];
            C2F(dset) (&setLen, &x[i], &(sco->internal.data[0][i][numberOfPoints + 1]), &i__1);

            sco->internal.data[1][i][numberOfPoints] = y[i];
            C2F(dset) (&setLen, &y[i], &(sco->internal.data[1][i][numberOfPoints + 1]), &i__1);

            sco->internal.data[2][i][numberOfPoints] = z[i];
            C2F(dset) (&setLen, &z[i], &(sco->internal.data[2][i][numberOfPoints + 1]), &i__1);
        }

        sco->internal.numberOfPoints++;
    }
}

static BOOL pushData(scicos_block * block, int row)
{
    char *pFigureUID;
    char *pAxeUID;
    char *pPolylineUID;

    static const int i__0 = 0;

    double *x;
    double *y;
    double *z;
    sco_data *sco;

    BOOL result = TRUE;

    pFigureUID = getFigure(block);
    pAxeUID = getAxe(pFigureUID, block);
    pPolylineUID = getPolyline(pAxeUID, block, row);

    sco = getScoData(block);
    if (sco == NULL)
        return FALSE;

    // select the right input and row
    x = sco->internal.data[0][row];
    y = sco->internal.data[1][row];
    z = sco->internal.data[2][row];

    result &= setGraphicObjectProperty(pPolylineUID, __GO_DATA_MODEL_X__, x, jni_double_vector, sco->internal.maxNumberOfPoints);
    result &= setGraphicObjectProperty(pPolylineUID, __GO_DATA_MODEL_Y__, y, jni_double_vector, sco->internal.maxNumberOfPoints);
    result &= setGraphicObjectProperty(pPolylineUID, __GO_DATA_MODEL_Z__, z, jni_double_vector, sco->internal.maxNumberOfPoints);

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
    int win_pos[2];
    int win_dim[2];

    int *ipar = block->ipar;
    int nipar = block->nipar;

    win_pos[0] = ipar[nipar - 5];
    win_pos[1] = ipar[nipar - 4];
    win_dim[0] = ipar[nipar - 3];
    win_dim[1] = ipar[nipar - 2];

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
    sco_data *sco = (sco_data *) * (block->work);

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
        sco->scope.cachedFigureUID = pFigureUID;

        // allocate the axes through the getter
        pAxe = getAxe(pFigureUID, block);

        /*
         * Setup according to block settings
         */
        setLabel(pAxe, __GO_X_AXIS_LABEL__, "x");
        setLabel(pAxe, __GO_Y_AXIS_LABEL__, "y");
        setLabel(pAxe, __GO_Z_AXIS_LABEL__, "z");

        setPolylinesBounds(block);
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
    sco_data *sco = (sco_data *) * (block->work);

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
        pAxe = cloneGraphicObject(getAxesModel());

        if (pAxe != NULL)
        {
            setGraphicObjectRelationship(pFigureUID, pAxe);

            // allocate the polylines through the getter
            for (i = 0; i < block->insz[0]; i++)
            {
                getPolyline(pAxe, block, i);
            }
        }
    }

    sco->scope.cachedAxeUID = pAxe;
    return pAxe;
}

static char *getPolyline(char *pAxeUID, scicos_block * block, int row)
{
    char *pPolyline;
    static const double d__0 = 0.0;
    static const int i__1 = 1;
    static const BOOL b__true = TRUE;

    int color;
    int markSize;
    double lineThickness;

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

            /*
             * Default setup (will crash if removed)
             */
            {
                int polylineSize[2] = { 1, block->ipar[2] };
                setGraphicObjectProperty(pPolyline, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, polylineSize, jni_int_vector, 2);
            }

            setGraphicObjectProperty(pPolyline, __GO_DATA_MODEL_X__, &d__0, jni_double_vector, 1);
            setGraphicObjectProperty(pPolyline, __GO_DATA_MODEL_Y__, &d__0, jni_double_vector, 1);
            setGraphicObjectProperty(pPolyline, __GO_DATA_MODEL_Z__, &d__0, jni_double_vector, 1);

            color = block->ipar[3 + row];
            markSize = block->ipar[3 + block->ipar[1] + row];
            lineThickness = (double)markSize;
            if (color > 0)
            {
                setGraphicObjectProperty(pPolyline, __GO_LINE_MODE__, &b__true, jni_bool, 1);

                setGraphicObjectProperty(pPolyline, __GO_LINE_COLOR__, &color, jni_int, 1);
                setGraphicObjectProperty(pPolyline, __GO_LINE_THICKNESS__, &lineThickness, jni_double, 1);
            }
            else
            {
                color = -color;
                setGraphicObjectProperty(pPolyline, __GO_MARK_MODE__, &b__true, jni_bool, 1);

                setGraphicObjectProperty(pPolyline, __GO_MARK_STYLE__, &color, jni_int, 1);
                setGraphicObjectProperty(pPolyline, __GO_MARK_SIZE__, &markSize, jni_int, 1);
            }
        }
    }

    if (sco->scope.cachedPolylinesUIDs != NULL)
    {
        sco->scope.cachedPolylinesUIDs[row] = pPolyline;
    }
    return pPolyline;
}

static BOOL setPolylinesBuffers(scicos_block * block, int maxNumberOfPoints)
{
    int i;

    char *pFigureUID;
    char *pAxeUID;
    char *pPolylineUID;

    BOOL result = TRUE;
    int polylineSize[2] = { 1, maxNumberOfPoints };

    pFigureUID = getFigure(block);
    pAxeUID = getAxe(pFigureUID, block);

    for (i = 0; i < block->insz[0]; i++)
    {
        pPolylineUID = getPolyline(pAxeUID, block, i);
        result &= setGraphicObjectProperty(pPolylineUID, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, polylineSize, jni_int_vector, 2);
    }

    return result;
}

static BOOL setPolylinesBounds(scicos_block * block)
{
    char *pFigureUID;
    char *pAxeUID;

    BOOL result;
    double dataBounds[6];
    double rotationAngle[2];

    dataBounds[0] = block->rpar[0]; // xMin
    dataBounds[1] = block->rpar[1]; // xMax
    dataBounds[2] = block->rpar[2]; // yMin
    dataBounds[3] = block->rpar[3]; // yMax
    dataBounds[4] = block->rpar[4]; // zMin
    dataBounds[5] = block->rpar[5]; // zMax

    rotationAngle[0] = block->rpar[6];  // alpha
    rotationAngle[1] = block->rpar[7];  // theta

    pFigureUID = getFigure(block);
    pAxeUID = getAxe(pFigureUID, block);

    result = setGraphicObjectProperty(pAxeUID, __GO_DATA_BOUNDS__, dataBounds, jni_double_vector, 6);
    result &= setGraphicObjectProperty(pAxeUID, __GO_ROTATION_ANGLES__, rotationAngle, jni_double_vector, 2);

    return result;
}
