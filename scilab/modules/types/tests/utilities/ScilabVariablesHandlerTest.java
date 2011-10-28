package Test;

import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabVariables;
import org.scilab.modules.types.ScilabVariablesHandler;

public class ScilabVariablesHandlerTest implements ScilabVariablesHandler {

    private static ScilabVariablesHandlerTest handlerTest;
    private String var;

    public void handle(ScilabType var) {
        this.var = var.toString();
    }

    public static final int startTest() {
        handlerTest = new ScilabVariablesHandlerTest();
        return ScilabVariables.addScilabVariablesHandler(handlerTest);
    }

    public static final void endTest() {
        ScilabVariables.removeScilabVariablesHandler(handlerTest);
        handlerTest = null;
    }

    public static final String getTestResult() {
        return handlerTest.var;
    }
}