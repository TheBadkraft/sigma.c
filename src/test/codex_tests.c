
#include "../open/test.h"
#include "../open/core.h"

#include "../codex.h"

//  TEST CASE prototypes
void _load_codex(void);

//  utility prototypes
void _output_codex_info(codex);

static directory cwd = NULL;

int main(int argc, string *argv)
{
    Directory.current(&cwd);

    BEGIN_SET(SigC.Codex, true)
    {
        TEST(_load_codex);
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