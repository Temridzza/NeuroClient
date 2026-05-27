#include "Markdown.h"

#include <cmark-gfm.h>
#include <cmark-gfm-core-extensions.h>
#include <mutex>

void initCmark()
{
    static std::once_flag flag;

    std::call_once(flag, []() {
        cmark_gfm_core_extensions_ensure_registered();
    });
}

QString markdownToHtml(const QString& md) {
    const QByteArray input = md.toUtf8();
    initCmark();

    cmark_parser* parser =
        cmark_parser_new(CMARK_OPT_DEFAULT);

    // 👉 ВКЛЮЧАЕМ TABLE EXTENSION
    cmark_syntax_extension* table_ext =
        cmark_find_syntax_extension("table");

    cmark_parser_attach_syntax_extension(parser, table_ext);

    cmark_parser_feed(parser, input.constData(), input.size());

    cmark_node* doc = cmark_parser_finish(parser);

    char* html = cmark_render_html(
        doc,
        CMARK_OPT_DEFAULT | CMARK_OPT_UNSAFE,
        nullptr
        );

    QString result = QString::fromUtf8(html);

    cmark_node_free(doc);
    cmark_parser_free(parser);
    free(html);

    return result;
}