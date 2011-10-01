#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
_XMLBuildAttrs (xml_attr * attrs)
{
    size_t      count;
    size_t      len;

    size_t      copied;

    char *      str;

    for (len = 0, count = 0; attrs[count].key; count++) {
        len += strlen (attrs[count].key) + strlen (attrs[count].value) + 3;
    }
    len += count;

    if ((str = malloc (len * sizeof (char))) == NULL) {
        return (NULL);
    }

    for (copied = 0, count = 0; attrs[count].key; count++) {
        copied += snprintf (&str[copied], len - copied, "%s=\"%s\" ", attrs[count].key, attrs[count].value);
    }

    return (str);
}

char *
XMLNode (const char * type, xml_attr * attrs, const char * format, ...)
{
    va_list             ap;
    char *              xml;
    int                 len;
    size_t              alloc;
    char *              content;
    char *              attrs_str;

    va_start (ap, format);
    content = _XMLBuildContent (format, ap);
    va_end (ap);
    if (content == NULL) {
        goto error;
    }

    if ((attrs_str = _XMLBuildAttrs (attrs)) == NULL) {
        goto free_content;
    }

    len = snprintf (NULL, 0, "<%s %s>%s</%s>", type, attrs_str, content, type);
    alloc = len + 1;
    if ((xml = malloc (alloc * sizeof (char))) == NULL) {
        goto free_attrs;
    }

    snprintf (xml, alloc, "<%s %s>%s</%s>", type, attrs_str ,content, type);
    free (attrs_str);
    free (content);
    return (xml);

  free_attrs:
    free (attrs_str);
  free_content:
    free (content);
  error:
    return (NULL);
}

#define ATTRS(...)                                                      \
    (xml_attr []){__VA_ARGS__, {NULL, NULL}}

int
main ()
{
    char * xml;

    xml = XMLNode ("a", ATTRS({"href", "#"}, {"class", "plop"}), "%s",
                   XMLNode ("span", ATTRS({"class", "plip"}), "%s%d", "coucou", 42));

    printf("%s\n", xml);
    free(xml);
    return (0);
}
