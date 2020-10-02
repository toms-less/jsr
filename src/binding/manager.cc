#include <include/binding.h>

binding::SystemFuncManager::SystemFuncManager(instance::IntanceManager &instance_manager) : instance_manager_(instance_manager)
{
}

bool binding::SystemFuncManager::initialize()
{
    /**
     * build system function of 'console'.
     * 1. 'console.log';
     * 2. 'console.info';
     * 3. 'console.warn';
     * 4. 'console.debug';
     * 5. 'console.error';
     * 6. 'console.time';
     * 7. 'console.timeEnd';
     * 
    */
    map_.insert(std::pair<std::string, base::v8_cb>("binding::Console::log", binding::Console::log));
    map_.insert(std::pair<std::string, base::v8_cb>("binding::Console::info", binding::Console::info));
    map_.insert(std::pair<std::string, base::v8_cb>("binding::Console::warn", binding::Console::warn));
    map_.insert(std::pair<std::string, base::v8_cb>("binding::Console::debug", binding::Console::debug));
    map_.insert(std::pair<std::string, base::v8_cb>("binding::Console::error", binding::Console::error));
    map_.insert(std::pair<std::string, base::v8_cb>("binding::Console::time", binding::Console::time));
    map_.insert(std::pair<std::string, base::v8_cb>("binding::Console::timeEnd", binding::Console::timeEnd));

    /**
     * build system function of 'http'.
     * 1. 'http.get';
     * 2. 'http.post';
     * 3. 'http.options()';
     * 4. 'http.patch()';
     * 5. 'http.put()';
     * 6. 'http.delete()';
     * 
    */
    map_.insert(std::pair<std::string, base::v8_cb>("binding::Http::get", binding::Http::get));
    map_.insert(std::pair<std::string, base::v8_cb>("binding::Http::post", binding::Http::post));
    map_.insert(std::pair<std::string, base::v8_cb>("binding::Http::options", binding::Http::options));
    map_.insert(std::pair<std::string, base::v8_cb>("binding::Http::patch", binding::Http::patch));
    map_.insert(std::pair<std::string, base::v8_cb>("binding::Http::put", binding::Http::put));
    map_.insert(std::pair<std::string, base::v8_cb>("binding::Http::del", binding::Http::del));

    auto instance_log = base::Log::instance_logger();
    for (instance::Instance *instance : instance_manager_.instances())
    {
        instance::BindObjectContext deps_ctx("jsr", "deps", binding::Binding::deps, &instance_manager_, &map_);
        instance->bind_object(deps_ctx);
        if (!deps_ctx.ok())
        {
            instance_log->error(deps_ctx.error());
            return false;
        }

        instance::BindObjectContext bind_ctx("jsr", "bind", binding::Binding::bind, &instance_manager_, &map_);
        instance->bind_object(bind_ctx);
        if (!bind_ctx.ok())
        {
            instance_log->error(bind_ctx.error());
            return false;
        }
    }

    /**
     * Read all the 'binding.js' files in the standard libraries directory.
     * 
    */
    std::string binding_path = base::Util::cwd() + "/binding";
    std::vector<std::string> std_files;
    base::Util::list_files(binding_path, std_files);
    if (std_files.size() == 0)
    {
        std::string error = "There is no standard libaries file in the standard libaries home '" + binding_path + "'.";
        instance_log->error(error);
        return false;
    }

    /**
     * Load the 'binding.js' of all the standard libraries. 
     * 
    */
    std::string bindfile = "binding.js";
    for (instance::Instance *instance : instance_manager_.instances())
    {
        for (std::string &file : std_files)
        {
            if (!base::Util::end_with(file, bindfile))
            {
                continue;
            }
            std::string bind_script = base::Util::read_file(file);
            instance::LoadBindingContext context(bind_script);
            instance->load_binding(context);
            if (!context.ok())
            {
                instance_log->error(context.error().c_str());
                return false;
            }
        }
    }
    return true;
}