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

#include <string.h>

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
 */
static void appendData(scicos_block * block, double *x, double *y);

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
SCICOS_BLOCKS_IMPEXP void canimxy(scicos_block * block, scicos_flag flag)
{
    char *pFigureUID;
    sco_data *sco;

    int j;
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

            appendData(block, (double *)block->inptr[0], (double *)block->inptr[1]);
            for (j = 0; j < block->insz[0]; j++)
            {
                result = pushData(block, j);
                if (result == FALSE)
                {
                    Coserror("%s: unable to push some data.", "cscopxy");
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

static void appendData(scicos_block * block, double *x, double *y)
{
    int i;

    sco_data *sco = (sco_data *) * (block->work);
    int maxNumberOfPoints = sco->internal.maxNumberOfPoints;
    int numberOfPoints = sco->internal.numberOfPoints;

    /*
     * Handle the case where the scope has more points than maxNumberOfPoints
     */
    if (sco != NULL && numberOfPoints >= maxNumberOfPoints)
    {
        unsigned int setLen = (unsigned int)maxNumberOfPoints - 1;

        // on a full scope, push data
        for (i = 0; i < block->insz[0]; i++)
        {
            memmove(sco->internal.data[0][i], &sco->internal.data[0][i][1], setLen * sizeof(double));
            sco->internal.data[0][i][setLen] = x[i];

            memmove(sco->internal.data[1][i], &sco->internal.data[1][i][1], setLen * sizeof(double));
            sco->internal.data[1][i][setLen] = y[i];
        }

        // then return
        return;
    }

    /*
     * Update data
     */
    if (sco != NULL)
    {
        int setLen;

        for (i = 0; i < block->insz[0]; i++)
        {
            for (setLen = maxNumberOfPoints - numberOfPoints - 1; setLen >= 0; setLen--)
            {
                sco->internal.data[0][i][numberOfPoints + setLen] = x[i];
                sco->internal.data[1][i][numberOfPoints + setLen] = y[i];
            }
        }

        sco->internal.numberOfPoints++;
    }
}

static BOOL pushData(scicos_block * block, int row)
{
    char *pFigureUID;
    char *pAxeUID;
    char *pPolylineUID;

    double *x;
    double *y;
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

    result &= setGraphicObjectProperty(pPolylineUID, __GO_DATA_MODEL_X__, x, jni_double_vector, sco->internal.maxNumberOfPoints);
    result &= setGraphicObjectProperty(pPolylineUID, __GO_DATA_MODEL_Y__, y, jni_double_vector, sco->internal.maxNumberOfPoints);

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

    win_pos[0] = ipar[6];
    win_pos[1] = ipar[7];
    win_dim[0] = ipar[8];
    win_dim[1] = ipar[9];

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

static char *getFigure(scicos_block * block)
{
    signed int figNum;
    char *pFigureUID = NULL;
    char *pAxe = NULL;
    int i__1 = 1;
    sco_data *sco = (sco_data *) * (block->work);

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

        setGraphicObjectProperty(pAxe, __GO_X_AXIS_VISIBLE__, &i__1, jni_bool, 1);
        setGraphicObjectProperty(pAxe, __GO_Y_AXIS_VISIBLE__, &i__1, jni_bool, 1);

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
        cloneAxesModel(pFigureUID);
        pAxe = findChildWithKindAt(pFigureUID, __GO_AXES__, 0);
    }

    /*
     * Setup on first access
     */
    if (pAxe != NULL)
    {
        // allocate the polylines through the getter
        for (i = 0; i < block->insz[0]; i++)
        {
            getPolyline(pAxe, block, i);
        }
    }

    /*
     * then cache
     */
    sco->scope.cachedAxeUID = pAxe;
    return pAxe;
}

static char *getPolyline(char *pAxeUID, scicos_block * block, int row)
{
    char *pPolyline;
    double d__0 = 0.0;
    BOOL b__true = TRUE;

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

        color = block->ipar[3];
        markSize = block->ipar[4];
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

    /*
     * then cache
     */
    if (sco->scope.cachedPolylinesUIDs != NULL)
    {
        sco->scope.cachedPolylinesUIDs[row] = pPolyline;
    }
    return pPolyline;
}

static BOOL setPolylinesBounds(scicos_block * block)
{
    char *pFigureUID;
    char *pAxeUID;

    double dataBounds[6];

    dataBounds[0] = block->rpar[0]; // xMin
    dataBounds[1] = block->rpar[1]; // xMax
    dataBounds[2] = block->rpar[2]; // yMin
    dataBounds[3] = block->rpar[3]; // yMax
    dataBounds[4] = -1.0;       // zMin
    dataBounds[5] = 1.0;        // zMax

    pFigureUID = getFigure(block);
    pAxeUID = getAxe(pFigureUID, block);

    return setGraphicObjectProperty(pAxeUID, __GO_DATA_BOUNDS__, dataBounds, jni_double_vector, 6);
}
