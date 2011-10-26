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
#include "math.h"

#include "localization.h"

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
        char *cachedFigureUID;
        char *cachedAxeUID;
        char *cachedPlot3dUID;
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
 * Push the block data to the polyline
 *
 * \param data the data to push
 *
 */
static BOOL pushData(scicos_block * block, double *data);

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
 * Get (and allocate on demand) the plot3d
 *
 * \param pAxeUID the parent axe UID
 * \param block the block
 * \return a valid plot3d UID or NULL on error
 */
static char *getPlot3d(char *pAxeUID, scicos_block * block);

/**
 * Set the plot3d and axes bounds
 *
 * \param block the block
 * \param pAxeUID the axe
 * \param pPlot3dUID the plot3d
 */
static BOOL setBounds(scicos_block * block, char *pAxeUID, char *pPlot3dUID);

/**
 * Set the plot3d settings
 *
 * \param block the block
 * \param pPlot3dUID the plot3d
 */
static BOOL setPlot3dSettings(scicos_block * block, char *pPlot3dUID);

/**
 * Set the plot3d default values
 *
 * \param block the block
 * \param pPlot3dUID the plot3d
 */
static BOOL setDefaultValues(scicos_block * block, char *pPlot3dUID);

/*****************************************************************************
 * Simulation function
 ****************************************************************************/

/** \fn void cmatview(scicos_block * block,int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An int which indicates the state of the block (init, update, ending)
*/
SCICOS_BLOCKS_IMPEXP void cmat3d(scicos_block * block, scicos_flag flag)
{
    char *pFigureUID;

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

        u = GetRealInPortPtrs(block, 1);

        result = pushData(block, u);
        if (result == FALSE)
        {
            Coserror("%s: unable to push some data.", "cmatview");
            break;
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
    BOOL result;

    if (sco == NULL)
    {
        /*
         * Data allocation
         */

        sco = (sco_data *) MALLOC(sizeof(sco_data));
        if (sco == NULL)
            goto error_handler_sco;

        sco->scope.cachedFigureUID = NULL;
        sco->scope.cachedAxeUID = NULL;
        sco->scope.cachedPlot3dUID = NULL;

        *(block->work) = sco;
    }

    return sco;

    /*
     * Error management (out of normal flow)
     */

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
        FREE(sco);
    }
}

static BOOL pushData(scicos_block * block, double *data)
{
    char *pFigureUID;
    char *pAxeUID;
    char *pPlot3dUID;

    BOOL result;
    int i;

    int m, n;

    pFigureUID = getFigure(block);
    pAxeUID = getAxe(pFigureUID, block);
    pPlot3dUID = getPlot3d(pAxeUID, block);

    m = GetInPortSize(block, 1, 1);
    n = GetInPortSize(block, 1, 2);

    result = setGraphicObjectProperty(pPlot3dUID, __GO_DATA_MODEL_Z__, data, jni_double_vector, m * n);

    return result;
}

/*****************************************************************************
 *
 * Graphic utils
 *
 ****************************************************************************/

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

        setGraphicObjectProperty(pFigureUID, __GO_COLORMAP__, block->rpar, jni_double_vector, block->ipar[2]);

        // allocate the axes through the getter
        pAxe = getAxe(pFigureUID, block);

        /*
         * Setup according to block settings
         */
        setLabel(pAxe, __GO_X_AXIS_LABEL__, "x");
        setLabel(pAxe, __GO_Y_AXIS_LABEL__, "y");
        setLabel(pAxe, __GO_Z_AXIS_LABEL__, "z");

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
        pAxe = createGraphicObject(__GO_AXES__);

        if (pAxe != NULL)
        {
            setGraphicObjectRelationship(pFigureUID, pAxe);
        }
    }

    if (sco->scope.cachedAxeUID == NULL)
    {
        sco->scope.cachedAxeUID = pAxe;
    }
    return pAxe;
}

static char *getPlot3d(char *pAxeUID, scicos_block * block)
{
    char *pPlot3d;
    static const double d__0 = 0.0;
    static const int i__0 = 0;
    static const BOOL b__true = TRUE;

    int color;

    sco_data *sco = (sco_data *) * (block->work);

    // fast path for an existing object
    if (sco->scope.cachedPlot3dUID != NULL)
    {
        return sco->scope.cachedPlot3dUID;
    }

    pPlot3d = findChildWithKindAt(pAxeUID, __GO_PLOT3D__, 0);

    /*
     * Allocate if necessary
     */
    if (pPlot3d == NULL)
    {
        pPlot3d = createGraphicObject(__GO_PLOT3D__);

        if (pPlot3d != NULL)
        {
            createDataObject(pPlot3d, __GO_PLOT3D__);
            setGraphicObjectRelationship(pAxeUID, pPlot3d);

            setBounds(block, pAxeUID, pPlot3d);
            setPlot3dSettings(block, pPlot3d);
            setDefaultValues(block, pPlot3d);
        }
    }

    if (sco->scope.cachedPlot3dUID == NULL)
    {
        sco->scope.cachedPlot3dUID = pPlot3d;
    }
    return pPlot3d;
}

static BOOL setBounds(scicos_block * block, char *pAxeUID, char *pPlot3dUID)
{
    BOOL result;

    int gridSize[4];
    double dataBounds[6];
    double rotationAngle[2];

    int m, n;
    int colormapLen;

    m = GetInPortSize(block, 1, 1);
    n = GetInPortSize(block, 1, 2);

    gridSize[0] = 1;
    gridSize[1] = n;
    gridSize[2] = 1;
    gridSize[3] = m;

    colormapLen = block->ipar[3];
    if (colormapLen == 1)
    {
        dataBounds[0] = 0;      // xMin
        dataBounds[1] = (double)n;  // xMax
        dataBounds[2] = 0;      // yMin
        dataBounds[3] = (double)m;  // yMax
    }
    else
    {
        dataBounds[0] = block->rpar[colormapLen + 0];   // xMin
        dataBounds[1] = block->rpar[colormapLen + 1];   // xMax
        dataBounds[2] = block->rpar[colormapLen + 2];   // yMin
        dataBounds[3] = block->rpar[colormapLen + 3];   // yMax
    }

    dataBounds[4] = (double)block->ipar[0]; // zMin
    dataBounds[5] = (double)block->ipar[1]; // zMax

    rotationAngle[0] = 80;      // alpha
    rotationAngle[1] = 250;     // theta

    result = setGraphicObjectProperty(pPlot3dUID, __GO_DATA_MODEL_GRID_SIZE__, gridSize, jni_int_vector, 4);
    result &= setGraphicObjectProperty(pAxeUID, __GO_DATA_BOUNDS__, dataBounds, jni_double_vector, 6);
    result &= setGraphicObjectProperty(pAxeUID, __GO_ROTATION_ANGLES__, rotationAngle, jni_double_vector, 2);

    return result;
}

static BOOL setPlot3dSettings(scicos_block * block, char *pPlot3dUID)
{
    static const int i__1 = 1;
    static const double d__1 = 1.0;
    static const int i__2 = 2;
    static const int i__4 = 4;

    BOOL result = TRUE;

    result &= setGraphicObjectProperty(pPlot3dUID, __GO_SURFACE_MODE__, &i__1, jni_bool, 1);
    result &= setGraphicObjectProperty(pPlot3dUID, __GO_LINE_THICKNESS__, &d__1, jni_double, 1);

    result &= setGraphicObjectProperty(pPlot3dUID, __GO_COLOR_MODE__, &i__2, jni_int, 1);
    result &= setGraphicObjectProperty(pPlot3dUID, __GO_COLOR_FLAG__, &i__1, jni_int, 1);
    result &= setGraphicObjectProperty(pPlot3dUID, __GO_HIDDEN_COLOR__, &i__4, jni_int, 1);

    return result;
}

static BOOL setDefaultValues(scicos_block * block, char *pPlot3dUID)
{
    int m, n, len;
    int i;
    double *values;

    BOOL result;

    m = GetInPortSize(block, 1, 1);
    n = GetInPortSize(block, 1, 2);

    /*
     * Share the same memory for 0 allocation (z) and incremented index (x and y)
     */
    values = (double *)CALLOC(n * m, sizeof(double));
    if (values == NULL)
    {
        return FALSE;
    }
    result = setGraphicObjectProperty(pPlot3dUID, __GO_DATA_MODEL_Z__, values, jni_double_vector, m * n);

    len = max(m, n);
    for (i = 1; i <= len; i++)
    {
        values[i] = (double)i;
    }

    result &= setGraphicObjectProperty(pPlot3dUID, __GO_DATA_MODEL_X__, values, jni_double_vector, n);
    result &= setGraphicObjectProperty(pPlot3dUID, __GO_DATA_MODEL_Y__, values, jni_double_vector, m);

    FREE(values);
    return result;
}
