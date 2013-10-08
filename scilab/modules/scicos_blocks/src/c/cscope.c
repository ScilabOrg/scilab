/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2012 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
#include "deleteGraphicObject.h"

#include "CurrentFigure.h"
#include "CurrentObject.h"

#include "scicos_block4.h"
#include "scicos.h"

#include "localization.h"
#include "os_strdup.h"

#include "FigureList.h"
#include "BuildObjects.h"
#include "AxesModel.h"

#define HISTORY_POINTS_THRESHOLD 4096

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
        double ***bufferCoordinates;
        int maxNumberOfPoints;
        double ***historyCoordinates;
    } internal;

    struct
    {
        int periodCounter;

        BOOL disableBufferUpdate;
        int historyUpdateCounter;

        char const* cachedFigureUID;
        char *cachedAxeUID;
        char **cachedBufferPolylinesUIDs;
        char **cachedHistoryPolylinesUIDs;
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
 * Realloc the history buffer data
 *
 * \param block the block
 * \param numberOfPoints realloc to store this number of points
 */
static sco_data *reallocHistoryBuffer(scicos_block * block, int numberOfPoints);

/**
 * Set values into the coordinates buffer.
 *
 * \param block the block
 * \param coordinates the buffer
 * \param numberOfPoints the number of points to set (actual)
 * \param bufferPoints the buffer size (max)
 * \param t the time to set
 * \param value the value to set
 */
static void setBuffersCoordinates(scicos_block * block, double* coordinates, const int numberOfPoints,
                                  const int bufferPoints, const double t, const double value);

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
 * \param history get the history polyline
 * \return a valid polyline UID or NULL on error
 */
static char *getPolyline(char *pAxeUID, scicos_block * block, int row, BOOL history);

/**
 * Delete all the buffer polylines.
 *
 * \param block the block
 */
static void deleteBufferPolylines(scicos_block * block);

/**
 * Set the polylines history size and push the history buffer
 *
 * \param block the block
 * \param input the input port index
 * \param maxNumberOfPoints the size of the buffer
 */
static BOOL pushHistory(scicos_block * block, int input, int maxNumberOfPoints);

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
            sco = getScoData(block);
            sco = reallocHistoryBuffer(block, sco->internal.maxNumberOfPoints + sco->internal.numberOfPoints);
            sco->scope.disableBufferUpdate = FALSE;
            sco->scope.historyUpdateCounter = 0;
            pushHistory(block, 0, sco->internal.maxNumberOfPoints);
            deleteBufferPolylines(block);
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
        {
            goto error_handler_sco;
        }

        // 0 points out of a block->ipar[2] points buffer
        sco->internal.numberOfPoints = 0;

        sco->internal.bufferCoordinates = (double ***)CALLOC(block->nin, sizeof(double **));
        if (sco->internal.bufferCoordinates == NULL)
        {
            goto error_handler_bufferCoordinates;
        }

        for (i = 0; i < block->nin; i++)
        {
            sco->internal.bufferCoordinates[i] = (double **)CALLOC(block->insz[i], sizeof(double *));
            if (sco->internal.bufferCoordinates[i] == NULL)
            {
                goto error_handler_bufferCoordinates_i;
            }
        }
        for (i = 0; i < block->nin; i++)
        {
            for (j = 0; j < block->insz[i]; j++)
            {
                sco->internal.bufferCoordinates[i][j] = (double *)CALLOC(3 * block->ipar[2], sizeof(double));

                if (sco->internal.bufferCoordinates[i][j] == NULL)
                {
                    goto error_handler_bufferCoordinates_ij;
                }
            }
        }

        // 0 points out of a 0 points history
        sco->internal.maxNumberOfPoints = 0;

        sco->internal.historyCoordinates = (double ***)CALLOC(block->nin, sizeof(double **));
        if (sco->internal.historyCoordinates == NULL)
        {
            goto error_handler_historyCoordinates;
        }

        for (i = 0; i < block->nin; i++)
        {
            sco->internal.historyCoordinates[i] = (double **)CALLOC(block->insz[i], sizeof(double *));
            if (sco->internal.historyCoordinates[i] == NULL)
            {
                goto error_handler_historyCoordinates_i;
            }
        }

        sco->scope.periodCounter = 0;

        // flag to avoid pushing the buffer each time
        sco->scope.disableBufferUpdate = FALSE;
        // counter use to delay the history push
        sco->scope.historyUpdateCounter = 0;

        sco->scope.cachedFigureUID = NULL;
        sco->scope.cachedAxeUID = NULL;
        sco->scope.cachedBufferPolylinesUIDs = (char **)CALLOC(block->insz[0], sizeof(char *));
        sco->scope.cachedHistoryPolylinesUIDs = (char **)CALLOC(block->insz[0], sizeof(char *));

        *(block->work) = sco;
    }

    return sco;

    /*
     * Error management (out of normal flow)
     */

error_handler_historyCoordinates_i:
    for (j = 0; j < i; j++)
    {
        FREE(sco->internal.historyCoordinates[j]);
    }
    FREE(sco->internal.historyCoordinates);
error_handler_historyCoordinates:
    i = block->nin - 1;
    j = block->insz[i] - 1;
error_handler_bufferCoordinates_ij:
    for (k = 0; k < i; k++)
    {
        for (l = 0; l < j; l++)
        {
            FREE(sco->internal.bufferCoordinates[k][l]);
        }
    }
    i = block->nin - 1;
error_handler_bufferCoordinates_i:
    for (j = 0; j < i; j++)
    {
        FREE(sco->internal.bufferCoordinates[j]);
    }
    FREE(sco->internal.bufferCoordinates);
error_handler_bufferCoordinates:
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
                if (sco->internal.historyCoordinates[i][j] != NULL)
                {
                    FREE(sco->internal.historyCoordinates[i][j]);
                }
                FREE(sco->internal.bufferCoordinates[i][j]);
            }
            FREE(sco->internal.historyCoordinates[i]);
            FREE(sco->internal.bufferCoordinates[i]);
        }
        FREE(sco->internal.historyCoordinates);
        FREE(sco->internal.bufferCoordinates);

        for (i = 0; i < block->insz[0]; i++)
        {
            FREE(sco->scope.cachedHistoryPolylinesUIDs[i]);
            FREE(sco->scope.cachedBufferPolylinesUIDs[i]);
        }
        FREE(sco->scope.cachedAxeUID);

        FREE(sco);
        *(block->work) = NULL;
    }
}

static sco_data *reallocHistoryBuffer(scicos_block * block, int numberOfPoints)
{
    sco_data *sco = (sco_data *) * (block->work);
    int i;

    double *ptr;
    int allocatedNumberOfPoints;

    int previousNumberOfPoints = sco->internal.maxNumberOfPoints;
    int numberOfCopiedPoints = numberOfPoints - sco->internal.maxNumberOfPoints;

    double *buffer;
    int bufferNumberOfPoints = block->ipar[2];
    int bufferNewPointInc;

    if (previousNumberOfPoints == 0)
    {
        allocatedNumberOfPoints = numberOfPoints;
        bufferNewPointInc = 0;
    }
    else
    {
        allocatedNumberOfPoints = numberOfPoints - 1;
        bufferNewPointInc = 1;
    }

    if (sco->scope.historyUpdateCounter <= 0)
    {
        if (numberOfPoints > HISTORY_POINTS_THRESHOLD)
        {
            sco->scope.disableBufferUpdate = TRUE;
            sco->scope.historyUpdateCounter = numberOfPoints / HISTORY_POINTS_THRESHOLD;
        }
        else
        {
            sco->scope.disableBufferUpdate = FALSE;
            sco->scope.historyUpdateCounter = 0;
        }
    }

    for (i = 0; i < block->insz[0]; i++)
    {
        ptr = (double *)MALLOC(3 * allocatedNumberOfPoints * sizeof(double));
        if (ptr == NULL)
        {
            goto error_handler;
        }

        // memcpy existing X-axis values from the history
        memcpy(ptr, sco->internal.historyCoordinates[0][i], previousNumberOfPoints * sizeof(double));
        // memcpy existing Y-axis values from the history
        memcpy(ptr + allocatedNumberOfPoints, sco->internal.historyCoordinates[0][i] + previousNumberOfPoints, previousNumberOfPoints * sizeof(double));
        // clear the last points, the Z-axis values
        memset(ptr + 2 * allocatedNumberOfPoints, 0, allocatedNumberOfPoints * sizeof(double));

        // then set the last points to the last values for X-axis and Y-axis values from the buffer points
        buffer = sco->internal.bufferCoordinates[0][i];
        memcpy(ptr + previousNumberOfPoints, buffer + bufferNewPointInc, (numberOfCopiedPoints - bufferNewPointInc) * sizeof(double));
        memcpy(ptr + allocatedNumberOfPoints + previousNumberOfPoints, buffer + bufferNumberOfPoints + bufferNewPointInc, (numberOfCopiedPoints - bufferNewPointInc) * sizeof(double));

        FREE(sco->internal.historyCoordinates[0][i]);
        sco->internal.historyCoordinates[0][i] = ptr;
    }

    sco->internal.maxNumberOfPoints = allocatedNumberOfPoints;
    return sco;

error_handler:
    freeScoData(block);
    // allocation error
    set_block_error(-5);
    return NULL;
}

static void setBuffersCoordinates(scicos_block* block, double* coordinates, const int numberOfPoints,
                                  const int bufferPoints, const double t, const double value)
{
    int setLen;
    sco_data *sco = (sco_data *) * (block->work);

    if (sco->scope.disableBufferUpdate == TRUE)
    {
        coordinates[numberOfPoints] = t;
        coordinates[bufferPoints + numberOfPoints] = value;
        return;
    }

    // X-axis values first
    for (setLen = numberOfPoints; setLen < bufferPoints; setLen++)
    {
        coordinates[setLen] = t;
    }
    // then Y-axis values
    for (setLen = numberOfPoints; setLen < bufferPoints; setLen++)
    {
        coordinates[bufferPoints + setLen] = value;
    }
    // then Z-axis values (always clear'ed)
}

static void appendData(scicos_block * block, int input, double t, double *data)
{
    int i;

    sco_data *sco = (sco_data *) * (block->work);

    /*
     * Handle the case where the t is greater than the data_bounds
     */
    if (t > ((sco->scope.periodCounter + 1) * block->rpar[3]))
    {
        sco->scope.periodCounter++;

        // set the buffer coordinates to the last point
        for (i = 0; i < block->insz[input]; i++)
        {
            sco->internal.bufferCoordinates[input][i][0] = sco->internal.bufferCoordinates[input][i][sco->internal.numberOfPoints - 1];
            sco->internal.bufferCoordinates[input][i][block->ipar[2]] = sco->internal.bufferCoordinates[input][i][block->ipar[2] + sco->internal.numberOfPoints - 1];
        }
        sco->internal.numberOfPoints = 1;

        // clear the history coordinates
        sco->internal.maxNumberOfPoints = 0;
        for (i = 0; i < block->insz[input]; i++)
        {
            if (sco->internal.historyCoordinates[input][i] != NULL)
            {
                FREE(sco->internal.historyCoordinates[input][i]);
                sco->internal.historyCoordinates[input][i] = NULL;
            }
        }

        // configure scope setting
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
    if (sco != NULL && sco->internal.numberOfPoints >= block->ipar[2])
    {
        int maxNumberOfPoints = sco->internal.maxNumberOfPoints;

        // on a full scope, re-alloc history coordinates
        maxNumberOfPoints = maxNumberOfPoints + block->ipar[2];
        sco = reallocHistoryBuffer(block, maxNumberOfPoints);

        // set the buffer coordinates to the last point
        for (i = 0; i < block->insz[input]; i++)
        {
            sco->internal.bufferCoordinates[input][i][0] = sco->internal.bufferCoordinates[input][i][block->ipar[2] - 1];
            sco->internal.bufferCoordinates[input][i][block->ipar[2]] = sco->internal.bufferCoordinates[input][i][2 * block->ipar[2] - 1];
        }
        sco->internal.numberOfPoints = 1;

        // reconfigure related graphic objects
        if (pushHistory(block, input, sco->internal.maxNumberOfPoints) == FALSE)
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
        for (i = 0; i < block->insz[input]; i++)
        {
            const double value = data[i];
            setBuffersCoordinates(block, sco->internal.bufferCoordinates[input][i], sco->internal.numberOfPoints, block->ipar[2], t, value);
        }

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

    pFigureUID = getFigure(block);
    pAxeUID = getAxe(pFigureUID, block, input);
    pPolylineUID = getPolyline(pAxeUID, block, row, FALSE);

    sco = getScoData(block);
    if (sco == NULL)
    {
        return FALSE;
    }

    // do not push any data if disabled
    if (sco->scope.disableBufferUpdate == TRUE)
    {
        return TRUE;
    }

    // select the right input and row
    data = sco->internal.bufferCoordinates[input][row];

    return setGraphicObjectProperty(pPolylineUID, __GO_DATA_MODEL_COORDINATES__, data, jni_double_vector, block->ipar[2]);
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
    char *label = NULL;

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

    label = GetLabelPtrs(block);
    if (label != NULL)
    {
        if (strlen(label) > 0)
        {
            setGraphicObjectProperty(pFigureUID, __GO_NAME__, label, jni_string, 1);
        }
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

    // assert the sco is not NULL
    if (sco == NULL)
    {
        return NULL;
    }

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
    else
    {
        // set configured parameters
        setFigureSettings(pFigureUID, block);
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

    // assert the sco is not NULL
    if (sco == NULL)
    {
        return NULL;
    }

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
            getPolyline(pAxe, block, i, TRUE);
        }
        for (i = 0; i < block->insz[input]; i++)
        {
            getPolyline(pAxe, block, i, FALSE);
        }
    }
    else
    {
        return NULL;
    }

    /*
     * then cache with local storage
     */
    sco->scope.cachedAxeUID = os_strdup(pAxe);
    releaseGraphicObjectProperty(__GO_PARENT__, pAxe, jni_string, 1);
    return sco->scope.cachedAxeUID;
}

static char *getPolyline(char *pAxeUID, scicos_block * block, int row, BOOL history)
{
    char *pPolyline;
    BOOL b__true = TRUE;

    int color;

    char** polylinesUIDs;
    int polylineIndex;

    sco_data *sco = (sco_data *) * (block->work);

    // assert the sco is not NULL
    if (sco == NULL)
    {
        return NULL;
    }

    if (!history)
    {
        polylinesUIDs = sco->scope.cachedBufferPolylinesUIDs;
        polylineIndex = block->insz[0] + row;
    }
    else
    {
        polylinesUIDs = sco->scope.cachedHistoryPolylinesUIDs;
        polylineIndex = row;
    }

    // assert that the structure is in a good shape
    if (polylinesUIDs == NULL)
    {
        return NULL;
    }

    // fast path for an existing object
    if (polylinesUIDs[row] != NULL)
    {
        return polylinesUIDs[row];
    }

    pPolyline = findChildWithKindAt(pAxeUID, __GO_POLYLINE__, polylineIndex);

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
        else
        {
            return NULL;
        }
    }

    /*
     * Setup on first access
     */

    /*
     * Default setup of the nGons property
     */
    {
        int nGons = 1;
        setGraphicObjectProperty(pPolyline, __GO_DATA_MODEL_NUM_GONS__, &nGons, jni_int, 1);
    }

    color = block->ipar[3 + row];
    if (color > 0)
    {
        setGraphicObjectProperty(pPolyline, __GO_LINE_MODE__, &b__true, jni_bool, 1);
        setGraphicObjectProperty(pPolyline, __GO_LINE_COLOR__, &color, jni_int, 1);
    }
    else
    {
        int iMarkSize = 4;
        color = -color;
        setGraphicObjectProperty(pPolyline, __GO_MARK_MODE__, &b__true, jni_bool, 1);
        setGraphicObjectProperty(pPolyline, __GO_MARK_STYLE__, &color, jni_int, 1);
        setGraphicObjectProperty(pPolyline, __GO_MARK_SIZE__, &iMarkSize, jni_int, 1);
    }

    {
        int iClipState = 1; //on
        setGraphicObjectProperty(pPolyline, __GO_CLIP_STATE__, &iClipState, jni_int, 1);
    }

    /*
     * then cache with local storage
     */
    polylinesUIDs[row] = os_strdup(pPolyline);
    releaseGraphicObjectProperty(__GO_PARENT__, pPolyline, jni_string, 1);
    return polylinesUIDs[row];
}

static void deleteBufferPolylines(scicos_block * block)
{
    int i, j;

    char *pPolylineUID;

    sco_data *sco;

    sco = getScoData(block);
    for (i = 0; i < block->nin; i++)
    {
        for (j = 0; j < block->insz[i]; j++)
        {
            pPolylineUID = sco->scope.cachedBufferPolylinesUIDs[j];
            deleteGraphicObject(pPolylineUID);
        }
    }
}

static BOOL pushHistory(scicos_block * block, int input, int maxNumberOfPoints)
{
    int i;

    char const* pFigureUID;
    char *pAxeUID;
    char *pPolylineUID;

    double *data;
    sco_data *sco;

    BOOL result = TRUE;

    sco = getScoData(block);
    pFigureUID = getFigure(block);
    pAxeUID = getAxe(pFigureUID, block, input);

    // push the data only if the counter == 0, decrement the counter if positive
    if (sco->scope.historyUpdateCounter > 0)
    {
        sco->scope.historyUpdateCounter--;
    }
    if (sco->scope.historyUpdateCounter > 0)
    {
        return result;
    }

    for (i = 0; i < block->insz[input]; i++)
    {
        pPolylineUID = getPolyline(pAxeUID, block, i, TRUE);

        data = sco->internal.historyCoordinates[input][i];
        result &= setGraphicObjectProperty(pPolylineUID, __GO_DATA_MODEL_COORDINATES__, data, jni_double_vector, maxNumberOfPoints);
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
