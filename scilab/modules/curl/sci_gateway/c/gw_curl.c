#ifdef __cplusplus
extern "C" {
#endif
#include <mex.h>
#include <sci_gateway.h>
#include <api_scilab.h>
#include <MALLOC.h>
static int direct_gateway(char *fname, void F(void))
{
    F();
    return 0;
};
extern Gatefunc curleasy_Init;
extern Gatefunc swig_this;
extern Gatefunc swig_ptr;
extern Gatefunc _wrap_curl_easy_init;
extern Gatefunc _wrap_curl_easy_setopt;
extern Gatefunc _wrap_curl_easy_perform;
extern Gatefunc _wrap_curl_easy_cleanup;
extern Gatefunc _wrap_curl_easy_getinfo;
extern Gatefunc _wrap_curl_easy_duphandle;
extern Gatefunc _wrap_curl_easy_reset;
extern Gatefunc _wrap_curl_easy_recv;
extern Gatefunc _wrap_curl_easy_send;
static GenericTable Tab[] =
{
    {(Myinterfun)sci_gateway, curleasy_Init, "curleasy_Init"},
    {(Myinterfun)sci_gateway, swig_this, "swig_this"},
    {(Myinterfun)sci_gateway, swig_ptr, "swig_ptr"},
    {(Myinterfun)sci_gateway, _wrap_curl_easy_init, "curl_easy_init"},
    {(Myinterfun)sci_gateway, _wrap_curl_easy_setopt, "curl_easy_setopt"},
    {(Myinterfun)sci_gateway, _wrap_curl_easy_perform, "curl_easy_perform"},
    {(Myinterfun)sci_gateway, _wrap_curl_easy_cleanup, "curl_easy_cleanup"},
    {(Myinterfun)sci_gateway, _wrap_curl_easy_getinfo, "curl_easy_getinfo"},
    {(Myinterfun)sci_gateway, _wrap_curl_easy_duphandle, "curl_easy_duphandle"},
    {(Myinterfun)sci_gateway, _wrap_curl_easy_reset, "curl_easy_reset"},
    {(Myinterfun)sci_gateway, _wrap_curl_easy_recv, "curl_easy_recv"},
    {(Myinterfun)sci_gateway, _wrap_curl_easy_send, "curl_easy_send"},
};

int gw_curl()
{
    Rhs = Max(0, Rhs);
    if (*(Tab[Fin - 1].f) != NULL)
    {
        if (pvApiCtx == NULL)
        {
            pvApiCtx = (StrCtx*)MALLOC(sizeof(StrCtx));
        }
        pvApiCtx->pstName = (char*)Tab[Fin - 1].name;
        (*(Tab[Fin - 1].f))(Tab[Fin - 1].name, Tab[Fin - 1].F);
    }
    return 0;
}
#ifdef __cplusplus
}
#endif
