#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct xml_attr {
    const char *        key;
    const char *        value;
} xml_attr;

char *
_XMLBuildContent (const char * format, va_list ap)
{
    char *              content;
    int                 len;
    va_list             ap_copy;

    va_copy(ap_copy, ap);
    len = vsnprintf (NULL, 0, format, ap_copy) + 1;
    va_end(ap_copy);

    if ((content = malloc (len * sizeof (char))) == NULL) {
        return (NULL);
    }
    vsnprintf (content, len, format, ap);
    return (content);
}

char *
XMLNode (const char * type, xml_attr * attrs, const char * format, ...)
{
    va_list             ap;
    char *              xml;
    int                 len;
    size_t              alloc;
    char *              content;

    (void) attrs;

    va_start (ap, format);
    content = _XMLBuildContent (format, ap);
    va_end (ap);

    if (content == NULL) {
        goto error;
    }

    len = snprintf (NULL, 0, "<%s>%s</%s>", type, content, type);
    alloc = len + 1;
    if ((xml = malloc (alloc * sizeof (char))) == NULL) {
        goto free_content;
    }

    snprintf (xml, alloc, "<%s>%s</%s>", type, content, type);
    free (content);
    return (xml);

  free_content:
    free (content);
  error:
    return (NULL);
}

int
main ()
{
    char * xml;

    xml = XMLNode ("a", (xml_attr []){{"href", "#"}, {"class", "plop"}}, "%s%d", "coucou", 42);

    printf("%s\n", xml);
    free(xml);
    return (0);
}
