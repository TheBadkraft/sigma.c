
#include "../open/test.h"
#include "../open/core.h"

#include "../codex.h"

//  TEST CASE prototypes
void _load_codex(void);
void _add_source(void);
void _load_source(void);

//  utility prototypes
void _output_codex_info(codex);

static directory cwd = NULL;

int main(int argc, string *argv)
{
    Directory.current(&cwd);

    BEGIN_SET(SigC.Codex, true)
    {
        TEST(_load_codex);
        TEST(_add_source);
        TEST(_load_source);
    }
    END_SET(SigC.Codex)

    TEST_STATS();

    Directory.free(cwd);
}

//		================================ TEST CASES ================================
void _load_codex()
{
    codex cdx;
    assert(Codex.init(cwd, &cdx));

    Codex.dispose();
}
void _add_source()
{
    writefln("dir: %s", cwd->path);
    string srcPath = "./.targets/001/main.C";

    // assert(Path.exists(srcPath));
    assert(Parser.add_source(srcPath));
    assert(Parser.clear_sources());
}
void _load_source()
{
    write_header("sigc::Parser.load_source: load source file");

    string srcPath = "./.targets/001/main.C";

    Parser.add_source(srcPath);
    assert(Parser.load_sources());

    Parser.clear_sources();
}