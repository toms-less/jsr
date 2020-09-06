#include <include/test.h>
#include <include/base.h>

int main(int argc, char *argv[])
{
    test::Client client(grpc::CreateChannel(
        "127.0.0.1:6666", grpc::InsecureChannelCredentials()));

    /**
     * Add a script in the jsr server.
     * 
    */
    protos::JavaScript *script = new protos::JavaScript();
    script->set_name("simple.js");
    script->set_function("simple");
    script->set_version("1.0");
    script->set_repository("http://127.0.0.1:8080");
    script->set_namespace_("scripts");
    script->set_specifier("//simple.js");

    protos::WorkspaceJsr *workspace = new protos::WorkspaceJsr();
    workspace->set_name("simple");
    workspace->set_repository("https://github.com/example/simple.git");
    script->set_allocated_workspace(workspace);

    protos::ScriptRequest *script_request = new protos::ScriptRequest();
    script_request->set_group("group1");
    script_request->set_unit("unit1");
    script_request->set_action(protos::ScriptRequest::ActionType::ScriptRequest_ActionType_ADD);
    script_request->set_allocated_javascript(script);

    protos::RuntimeRequest add_script_req;
    add_script_req.set_id(std::to_string(base::Util::timestamp()));
    add_script_req.set_type(protos::Common_Type::Common_Type_SCRIPT);
    add_script_req.mutable_script()->AddAllocated(script_request);

    protos::RuntimeResponse add_script_res;
    test::RouteContext add_script_ctx(add_script_req, add_script_res);
    client.route(add_script_ctx);
    if (!add_script_ctx.ok())
    {
        std::cout << add_script_ctx.error() << std::endl;
        return 0;
    }
    std::cout << "--------" << add_script_res.message() << std::endl;

    /**
     * Build parameters.
     * 
    */
    protos::RuntimeRequest request;
    request.set_id(std::to_string(base::Util::timestamp()));
    request.set_type(protos::Common_Type::Common_Type_CALL);

    protos::CallRequest *call = new protos::CallRequest();
    call->set_function("simple");
    call->set_version("1.0");
    call->set_group("group1");
    call->set_unit("unit1");
    call->set_timeout(10000);
    call->set_method("POST");
    call->set_contenttype("application/json");
    (*call->mutable_headers())["sum1"] = "1";
    (*call->mutable_headers())["sum2"] = "2";
    (*call->mutable_headers())["div1"] = "100";
    (*call->mutable_headers())["div2"] = "2";
    (*call->mutable_parameters())["p1"] = "v1";
    call->set_data("{'key1':'value1','key2':'value2','key3':'value3'}");
    request.set_allocated_call(call);

    protos::RuntimeResponse response;

    /**
     * Build context.
     * 
    */
    test::RouteContext context(request, response);

    /**
     * Request server.
     * 
    */
    client.route(context);
    if (!context.ok())
    {
        std::cout << context.error() << std::endl;
        return 0;
    }

    /**
     * print all responsed messages.
     * 
    */
    std::cout << "response id: " << response.id() << std::endl;
    std::cout << "response type: " << response.type() << std::endl;
    std::cout << "response status: " << response.status() << std::endl;
    std::cout << "response message: " << response.message() << std::endl;
    std::cout << "response call function: " << response.call().function() << std::endl;
    std::cout << "response call version: " << response.call().version() << std::endl;
    std::cout << "response call group: " << response.call().group() << std::endl;
    std::cout << "response call unit: " << response.call().unit() << std::endl;
    std::cout << "response call runtime: " << response.call().runtime() << std::endl;
    std::cout << "response call status: " << response.call().status() << std::endl;
    std::cout << "response call contentType: " << response.call().contenttype() << std::endl;
    const google::protobuf::Map<std::string, std::string> &headers = response.call().headers();
    for (auto &pair : headers)
    {
        std::cout << "response call header: " << pair.first << ":" << pair.second << std::endl;
    }
    const google::protobuf::RepeatedPtrField<protos::Cookie> &cookies = response.call().cookies();
    if (cookies.size() > 0)
    {
        for (int i = 0; i < cookies.size(); i++)
        {
            const protos::Cookie &cookie = cookies.Get(i);
            std::string str = "{'name':'" + cookie.name() + "', 'value':'" + cookie.value() +
                              "', 'domain':'" + cookie.domain() + "', 'path': '" + cookie.domain() +
                              "', 'expires':'" + std::to_string(cookie.expires()) + "', 'maxAge':'" +
                              std::to_string(cookie.maxage()) + "', 'secure':'" + std::to_string(cookie.secure()) +
                              "', 'httpOnly':'" + std::to_string(cookie.httponly()) + "'}";
            std::cout << "response call cookie: " << str << std::endl;
        }
    }
    std::cout << "response call data: " << response.call().data() << std::endl;
    return 0;
}