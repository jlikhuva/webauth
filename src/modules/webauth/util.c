
/*
 * utility stuff
 */

#include "mod_webauth.h"

/*
 * get a required char* attr from a token, with logging if not present.
 * returns value or NULL on error,
 */
char *
mwa_get_str_attr(WEBAUTH_ATTR_LIST *alist, 
                 const char *name, 
                 request_rec *r, 
                 const char *func,
                 int *vlen)
{
    int status, i;

    status = webauth_attr_list_find(alist, name, &i);
    if (i == -1) {
        ap_log_error(APLOG_MARK, APLOG_ERR, 0, r->server,
                     "mod_webauth: %s: can't find attr(%s) in attr list",
                     func, name);
        return NULL;
    }
    if (vlen) 
        *vlen = alist->attrs[i].length;

    return (char*)alist->attrs[i].value;
}

/*
 * get note from main request 
 */
const char *
mwa_get_note(request_rec *r, const char *note)
{
    if (r->main) {
        return apr_table_get(r->main->notes, note);
    } else {
        return apr_table_get(r->notes, note);
    }
}

/*
 * remove note from main request, and return it if it was set, or NULL
 * if unset
 */
char *
mwa_remove_note(request_rec *r, const char *note)
{
    const char *val;
    if (r->main)
        r = r->main;

    val = apr_table_get(r->notes, note);

    if (val != NULL)
        apr_table_unset(r->notes, note);

    return (char*)val;
}

/*
 * set note in main request. does not make copy of data
 */
void
mwa_setn_note(request_rec *r, const char *note, const char *val)
{
    if (r->main) {
        apr_table_setn(r->main->notes, note, val);
    } else {
        apr_table_setn(r->notes, note, val);
    }
}


/*
 * log interesting stuff from the request
 */
void 
mwa_log_request(request_rec *r, const char *msg)
{
    /* FIXME: disabling for now */
    return;

#define LOG_S(a,b) ap_log_error(APLOG_MARK, APLOG_ERR, 0, r->server, \
              "mod_webauth: %s(%s)", a, (b != NULL)? b:"(null)");
#define LOG_D(a,b) ap_log_error(APLOG_MARK, APLOG_ERR, 0, r->server, \
              "mod_webauth: %s(%d)", a, b);

    ap_log_error(APLOG_MARK, APLOG_ERR, 0, r->server,
                 "mod_webauth: -------------- %s ------------------", msg);

    LOG_S("ap_auth_type", ap_auth_type(r));
    LOG_S("the_request", r->the_request);
    LOG_S("unparsed_uri", r->unparsed_uri);
    LOG_S("uri", r->uri);
    LOG_S("filename", r->filename);
    LOG_S("canonical_filename", r->canonical_filename);
    LOG_S("path_info", r->path_info);
    LOG_S("args", r->args);
    LOG_D("rpu->is_initialized", r->parsed_uri.is_initialized);
    LOG_S("rpu->query", r->parsed_uri.query);

    ap_log_error(APLOG_MARK, APLOG_ERR, 0, r->server,
                 "mod_webauth: -------------- %s ------------------", msg);

#undef LOG_S
#undef LOG_D
}

/*
 * get a WEBAUTH_KRB5_CTXT
 */
WEBAUTH_KRB5_CTXT *
mwa_get_webauth_krb5_ctxt(request_rec *r, const char *mwa_func)
{
    WEBAUTH_KRB5_CTXT *ctxt;
    int status;

    status = webauth_krb5_new(&ctxt);
    if (status != WA_ERR_NONE) {
        mwa_log_webauth_error(r, status, ctxt, mwa_func, "webauth_krb5_new");
        if (status == WA_ERR_KRB5)
            webauth_krb5_free(ctxt);
        return NULL;
    }
    return ctxt;
}


void
mwa_log_webauth_error(request_rec *r, 
                       int status, 
                      WEBAUTH_KRB5_CTXT *ctxt,
                      const char *mwa_func,
                      const char *func)
{
    if (status == WA_ERR_KRB5 && ctxt != NULL) {
        ap_log_error(APLOG_MARK, APLOG_ERR, 0, r->server,
                     "mod_webauth: %s: %s failed: %s (%d): %s %d",
                     mwa_func, func,
                     webauth_error_message(status), status,
                     webauth_krb5_error_message(ctxt), 
                     webauth_krb5_error_code(ctxt));
    } else {
        ap_log_error(APLOG_MARK, APLOG_ERR, 0, r->server,
                     "mod_webauth: %s: %s failed: %s (%d)",
                     mwa_func,
                     func,
                     webauth_error_message(status), status);
    }
}
