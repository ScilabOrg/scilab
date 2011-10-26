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
        double *ballsSize;
        double **data;
    } internal;

    struct
    {
        char *cachedFigureUID;
        char *cachedAxeUID;
        char **cachedArcsUIDs;
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
 * Get (and allocate on demand) the arc associated with the row
 *
 * \param pAxeUID the parent axe UID
 * \param block the block
 * \param row the current row index (0-indexed)
 * \return a valid polyline UID or NULL on error
 */
static char *getArc(char *pAxeUID, scicos_block * block, int row);

/**
 * Set the bounds
 *
 * \param block the block
 */
static BOOL setBounds(scicos_block * block);

/*****************************************************************************
 * Simulation function
 ****************************************************************************/

/** \fn void bouncexy(scicos_block * block,int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An int which indicates the state of the block (init, update, ending)
*/
SCICOS_BLOCKS_IMPEXP void bouncexy(scicos_block * block, scicos_flag flag)
{
    char *pFigureUID;

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

        appendData(block, (double *)block->inptr[0], (double *)block->inptr[1]);
        for (j = 0; j < block->insz[0]; j++)
        {
            result = pushData(block, j);
            if (result == FALSE)
            {
                Coserror("%s: unable to push some data.", "bouncexy");
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

        sco->internal.ballsSize = (double *)CALLOC(block->nin, sizeof(double));
        if (sco->internal.ballsSize == NULL)
            goto error_handler_ballsSize;
        for (i = 0; i < block->insz[0]; i++)
        {
            sco->internal.ballsSize[i] = block->z[6 * i + 2];
        }

        sco->internal.data = (double **)CALLOC(block->insz[0], sizeof(double *));
        if (sco->internal.data == NULL)
            goto error_handler_data;

        for (i = 0; i < block->insz[0]; i++)
        {
            sco->internal.data[i] = (double *)CALLOC(3, sizeof(double));
            if (sco->internal.data[i] == NULL)
                goto error_handler_data_i;
        }

        sco->scope.cachedFigureUID = NULL;
        sco->scope.cachedAxeUID = NULL;

        sco->scope.cachedArcsUIDs = (char **)CALLOC(block->insz[0], sizeof(char *));

        *(block->work) = sco;
    }

    return sco;

    /*
     * Error management (out of normal flow)
     */

error_handler_data_i:
    for (j = 0; j < i; j++)
    {
        FREE(sco->internal.data[i]);
    }
    FREE(sco->internal.data);
error_handler_data:
    FREE(sco->internal.ballsSize);
error_handler_ballsSize:
    FREE(sco);
error_handler_sco:
    // allocation error
    set_block_error(-5);
    return NULL;
}

static void freeScoData(scicos_block * block)
{
    sco_data *sco = (sco_data *) * (block->work);
    int i;

    if (sco != NULL)
    {
        for (i = 0; i < block->insz[0]; i++)
        {
            FREE(sco->internal.data[i]);
        }

        FREE(sco->internal.data);
        FREE(sco->internal.ballsSize);

//      Commented due to the C++ allocation
//      see http://bugzilla.scilab.org/show_bug.cgi?id=9747
//      FREE(sco->scope.cachedFigureUID);
//      sco->scope.cachedFigureUID = NULL;
//      for (i=0; i<block->nin; i++) {
//          for (j=0; j<block->insz[i]; j++) {
//              FREE(sco->scope.cachedArcsUIDs[i][j]);
//              sco->scope.cachedArcsUIDs[i][j] = NULL;
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
    double *upperLeftPoint;
    double ballSize;
    sco_data *sco = (sco_data *) * (block->work);

    /*
     * Update data
     */
    if (sco != NULL)
    {
        for (i = 0; i < block->insz[0]; i++)
        {
            upperLeftPoint = sco->internal.data[i];
            ballSize = sco->internal.ballsSize[i];

            upperLeftPoint[0] = x[i] - (ballSize / 2);  // x
            upperLeftPoint[1] = y[i] + (ballSize / 2);  // y
            upperLeftPoint[2] = 0;  // z
        }
    }
}

static BOOL pushData(scicos_block * block, int row)
{
    char *pFigureUID;
    char *pAxeUID;
    char *pArcUID;

    double *upperLeftPoint;
    sco_data *sco;

    pFigureUID = getFigure(block);
    pAxeUID = getAxe(pFigureUID, block);
    pArcUID = getArc(pAxeUID, block, row);

    sco = getScoData(block);
    if (sco == NULL)
        return FALSE;

    upperLeftPoint = sco->internal.data[row];
    return setGraphicObjectProperty(pArcUID, __GO_UPPER_LEFT_POINT__, upperLeftPoint, jni_double_vector, 3);
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

        sco->scope.cachedFigureUID = pFigureUID;

        // allocate the axes through the getter
        pAxe = getAxe(pFigureUID, block);

        /*
         * Setup according to block settings
         */
        setLabel(pAxe, __GO_X_AXIS_LABEL__, "x");
        setLabel(pAxe, __GO_Y_AXIS_LABEL__, "y");

        setBounds(block);
    }

    if (sco->scope.cachedFigureUID == NULL)
    {
        sco->scope.cachedFigureUID = pFigureUID;
    }
    return pFigureUID;
}

static char *getAxe(char *pFigureUID, scicos_block * block)
{
    static const BOOL b__f = FALSE;

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

            /*
             * Setup
             */
            setGraphicObjectProperty(pAxe, __GO_X_AXIS_VISIBLE__, &b__f, jni_bool, 1);
            setGraphicObjectProperty(pAxe, __GO_Y_AXIS_VISIBLE__, &b__f, jni_bool, 1);

            // allocate the polylines through the getter
            for (i = 0; i < block->insz[0]; i++)
            {
                getArc(pAxe, block, i);
            }
        }
    }

    sco->scope.cachedAxeUID = pAxe;
    return pAxe;
}

static char *getArc(char *pAxeUID, scicos_block * block, int row)
{
    static double d__0 = 0.0;
    static double d__2PI = 2 * M_PI;

    char *pArc;
    int color;

    sco_data *sco = (sco_data *) * (block->work);

    // fast path for an existing object
    if (sco->scope.cachedArcsUIDs != NULL && sco->scope.cachedArcsUIDs[row] != NULL)
    {
        return sco->scope.cachedArcsUIDs[row];
    }

    pArc = findChildWithKindAt(pAxeUID, __GO_ARC__, row);

    /*
     * Allocate if necessary
     */
    if (pArc == NULL)
    {
        pArc = createGraphicObject(__GO_ARC__);

        if (pArc != NULL)
        {
            createDataObject(pArc, __GO_ARC__);
            setGraphicObjectRelationship(pAxeUID, pArc);

            /*
             * Default setup
             */
            setGraphicObjectProperty(pArc, __GO_START_ANGLE__, &d__0, jni_double, 1);
            setGraphicObjectProperty(pArc, __GO_END_ANGLE__, &d__2PI, jni_double, 1);

            color = block->ipar[2 + row];
            setGraphicObjectProperty(pArc, __GO_BACKGROUND__, &color, jni_int, 1);

            setGraphicObjectProperty(pArc, __GO_WIDTH__, &sco->internal.ballsSize[row], jni_double, 1);
            setGraphicObjectProperty(pArc, __GO_HEIGHT__, &sco->internal.ballsSize[row], jni_double, 1);
        }
    }

    if (sco->scope.cachedArcsUIDs != NULL)
    {
        sco->scope.cachedArcsUIDs[row] = pArc;
    }
    return pArc;
}

static BOOL setBounds(scicos_block * block)
{
    char *pFigureUID;
    char *pAxeUID;

    BOOL result;
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
