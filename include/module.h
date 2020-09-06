#pragma once
#include <include/base.h>

/**
 * This namespace is about to JavaScript
 * modules.
 * 
*/
namespace module
{
    /**
     * 1. 'lib1.js'. In this example as below,
     * 'DepsType' is 'LOCAL'.
     * ---lib1.js---
     * import {foo} from '//mod/test.js'
     * 
     * 2. 'lib2.js'. In this example as below,
     * 'DepsType' is 'HTTP'.
     * ---lib2.js---
     * import {foo} from 'https://test.com/mod/test.js'
     *
     * 3. 'lib3.js'. In this example as below,
     * 'DepsType' is 'FTP'.
     * ---lib3.js---
     * import {foo} from 'ftp://test.com/mod/test.js' 
     *
     * 4. 'lib4.js'. In this example as below,
     * 'DepsType' is 'SYS'.
     * ---lib4.js---
     * import {foo1,foo2} from '@toms//std/net.js'
     * 
     * 5. 'lib5.js'. In this example as below,
     * 'DepsType' is 'THIRD_PARTY'.
     * ---lib5.js---
     * import {foo1,foo2} from '@redux//lib/test.js' 
     *
     */
    enum DepsType
    {
        LOCAL,
        HTTP,
        FTP,
        SYS,
        THIRD_PARTY,
        UNKNOWN
    };

    /**
     * Context of dependency parsing.
     * 
    */
    class DepsParseContext
    {
    public:
        DepsParseContext(const char *repository, const char *ns, const char *project, const char *specifier);
        std::string &repository();
        std::string &specifier();
        std::string &repo_ns();
        std::string &project();

        void set_type(DepsType type);
        DepsType &type();

        void set_script(const char *script);
        std::string &script();

        void set_ok();
        bool &ok();

        void set_error(const char *error);
        std::string &error();

    private:
        /**
         * Repository is the root path of current project.
         * It is used to make the absolutely module path
         * to retrieve the script content.
         * 
        */
        std::string repository_;

        /**
         * Namesapce is the domain of projects and dependencies.
         * In this system, there are maybe a lot of project
         * and dependencies. To avoid the same name of projects
         * or dependencies, jsr uses namespace as the domain of
         * projects and dependencies.
         * 
        */
        std::string namespace_;

        /**
         * v8 module specifier string.
         * In this example as below,
         * 'specifier_' value is '//mod/test.js'.
         * 
         * ---lib1.js---
         * import {foo} from '//mod/test.js'
        */
        std::string specifier_;

        /**
         * Project name.
         * 
        */
        std::string project_;

        /**
         * Dependency type.
         * 
        */
        DepsType type_;

        /**
         * Parsed script of this specifier.
         * 
        */
        std::string script_;

        /**
         * Parse status.
         * 
        */
        bool ok_;

        /**
         * Error message during the dependencies parsing.
         * 
        */
        std::string error_;
    };

    /**
     * JavaScript module.
     * 
    */
    class ScriptModule
    {
    public:
        /**
         * Resolve modules.
         * 
         */
        static v8::MaybeLocal<v8::Module> resolve(v8::Local<v8::Context> context,
                                                  v8::Local<v8::String> specifier, v8::Local<v8::Module> referrer);
        /**
         * Parse dependencies
         * 
        */
        void deps_parse(DepsParseContext &context);

    private:
        /**
         * Parse local dependency.
         * 
        */
        void local_deps_parse(DepsParseContext &context);

        /**
         * Parse HTTP dependency.
         * 
        */
        void http_deps_parse(DepsParseContext &context);

        /**
         * Parse FTP dependency.
         * 
        */
        void ftp_deps_parse(DepsParseContext &context);

        /**
         * Parse system dependency.
         * 
        */
        void sys_deps_parse(DepsParseContext &context);

        /**
         * Parse third party dependency.
         * 
        */
        void third_deps_parse(DepsParseContext &context);
    };

} // namespace module