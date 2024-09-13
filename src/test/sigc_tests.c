
#include "../open/test.h"
#include "../open/core.h"

#include "../sigmac.h"
#include "sigc_tests.h"

//  TEST CASE prototypes
void _get_sigc_instance(void);
void _load_arg_help(void);
void _load_fail_invalid_key(void);
void _load_fail_invalid_tag(void);
void _execute_paramless_option(void);
void _parameterize_source(void);

//  utility prototypes
size_t _count_options(sigC);
void _output_sigmac_info(sigC);

static string defPath = "./.data/sigmac.def";
file fDef = NULL;

int main(int argc, string *argv)
{
    if (Utils.path_exists(defPath))
    {
        fDef = File.new(defPath);
    }

    BEGIN_SET(Sigma.C, true)
    {
        TEST(_get_sigc_instance);
        TEST(_load_arg_help);
        TEST(_load_fail_invalid_key);
        TEST(_load_fail_invalid_tag);
        TEST(_execute_paramless_option);
        TEST(_parameterize_source);
    }
    END_SET(Sigma.C)

    TEST_STATS();

    File.free(fDef);
}
void _output_sigmac_info(sigC sigc)
{
    writefln("is initialized: %s", YN(sigc->is_initialized));
    writefln("loaded options: %ld", _count_options(sigc));

    if (sigc->error != NO_ERR)
    {
        string errMsg;
        String.new(0, &errMsg);
        SC.get_error(&errMsg);
        writefln("error:\n      %s", errMsg);
        String.free(errMsg);
    }
}

size_t _count_options(sigC sigc)
{
    size_t count = 0;

    if (sigc->options != NULL)
    {
        sigC_option ndx = sigc->options;
        while ((*ndx).opt_type != -1)
        {
            *++ndx;
        }

        count = ndx - sigc->options;
    }

    return count;
}

//		================================ TEST CASES ================================
void _get_sigc_instance()
{
    sigC sigc;
    assert(SC.instance(&sigc));

    _output_sigmac_info(sigc);
    sigc->dispose();
}
void _load_arg_help()
{
    sigC sigc;
    SC.instance(&sigc);

    //  simulating argv means the first arg is the command line executable, last element should be NULL
    string *argv = (string[]){"sigma.c", "--help", NULL};
    assert(sigc->load(argv));
    assert(_count_options(sigc) == 1);

    _output_sigmac_info(sigc);
    sigc->dispose();
}
void _load_fail_invalid_key()
{
    sigC sigc;
    SC.instance(&sigc);

    string *argv = (string[]){"sigma.c", "-X", NULL};

    assert(!sigc->load(argv));

    _output_sigmac_info(sigc);
    sigc->dispose();
}
void _load_fail_invalid_tag()
{
    sigC sigc;
    SC.instance(&sigc);

    string *argv = (string[]){"sigma.c", "--Foo", NULL};

    assert(!sigc->load(argv));

    _output_sigmac_info(sigc);
    sigc->dispose();
}
void _execute_paramless_option()
{
    sigC sigc;
    SC.instance(&sigc);

    string *argv = (string[]){"sigma.c", "--info", NULL};
    sigc->load(argv);

    sigC_option option = sigc->options;
    //  --info returns false (so further configuration will not follow)
    assert(!option->configure(NULL));

    _output_sigmac_info(sigc);
    sigc->dispose();
}
void _parameterize_source()
{
    sigC sigc;
    SC.instance(&sigc);

    string *argv = (string[]){"sigma.c", "./.targets/001/main.C", NULL};
    sigc->load(argv);
}