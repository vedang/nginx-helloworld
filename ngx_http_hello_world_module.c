#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

/* Setup function for hello_world directive */
static char *ngx_http_hello_world (ngx_conf_t *cf,     /* arguments passed to directive */
                                   ngx_command_t *cmd, /* current command structure */
                                   void *conf);        /* my custom config structure */

/* Setup function for the diff_hello_world directive */
static char *ngx_http_diff_hello_world (ngx_conf_t *cf,
                                        ngx_command_t *cmd,
                                        void *conf);

/* directives defined in the module */
static ngx_command_t  ngx_http_hello_world_commands[] = {
    { ngx_string ("hello_world"),           /* Name used to invoke the directive */
      NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS,  /* type: indicates where directive is legal
                                               and how many arguments it takes */
      ngx_http_hello_world,                 /* function to act on arguments passed
                                               to directive and convert them into
                                               user's configuration structure */
      0,                                    /* conf pointer */
      0,                                    /* offset pointer */
      NULL },

    { ngx_string ("diff_hello_world"),
      NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS,
      ngx_http_diff_hello_world,
      0,
      0,
      NULL },

    ngx_null_command
};

/* The Module Context - for managing the configurations */
static ngx_http_module_t  ngx_http_hello_world_module_ctx = {
    NULL,                          /* preconfiguration */
    NULL,                          /* postconfiguration */

    NULL,                          /* create main configuration */
    NULL,                          /* init main configuration */

    NULL,                          /* create server configuration */
    NULL,                          /* merge server configuration */

    NULL,                          /* create location configuration */
    NULL                           /* merge location configuration */
};

/* The Module Definition - the master control */
ngx_module_t ngx_http_hello_world_module = {
    NGX_MODULE_V1,
    &ngx_http_hello_world_module_ctx, /* module context */
    ngx_http_hello_world_commands,    /* module directives */
    NGX_HTTP_MODULE,                  /* module type */
    NULL,                             /* init master */
    NULL,                             /* init module */
    NULL,                             /* init process */
    NULL,                             /* init thread */
    NULL,                             /* exit thread */
    NULL,                             /* exit process */
    NULL,                             /* exit master */
    NGX_MODULE_V1_PADDING
};

static u_char  ngx_hello_world[] = "hello world";
static u_char  ngx_diff_hello_world[] = "My hello world. This is different";

static ngx_int_t ngx_http_hello_world_handler(ngx_http_request_t *r)
{
    ngx_buf_t    *b;
    ngx_chain_t   out;

    r->headers_out.content_type.len = sizeof("text/plain") - 1;
    r->headers_out.content_type.data = (u_char *) "text/plain";

    b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));

    out.buf = b;
    out.next = NULL;

    b->pos = ngx_hello_world;
    b->last = ngx_hello_world + sizeof(ngx_hello_world);
    b->memory = 1;
    b->last_buf = 1;

    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = sizeof(ngx_hello_world);
    ngx_http_send_header(r);

    return ngx_http_output_filter(r, &out);
}

static ngx_int_t ngx_http_diff_hello_world_handler(ngx_http_request_t *r)
{
    ngx_buf_t    *b;
    ngx_chain_t   out;

    r->headers_out.content_type.len = sizeof("text/plain") - 1;
    r->headers_out.content_type.data = (u_char *) "text/plain";

    b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));

    out.buf = b;
    out.next = NULL;

    b->pos = ngx_diff_hello_world;
    b->last = ngx_diff_hello_world + sizeof(ngx_diff_hello_world);
    b->memory = 1;
    b->last_buf = 1;

    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = sizeof(ngx_diff_hello_world);
    ngx_http_send_header(r);

    return ngx_http_output_filter(r, &out);
}

static char *ngx_http_hello_world(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t  *clcf;

    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_hello_world_handler;

    return NGX_CONF_OK;
}

static char *ngx_http_diff_hello_world(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t  *clcf;

    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_diff_hello_world_handler;

    return NGX_CONF_OK;
}
