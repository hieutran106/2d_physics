import lldb


def vec2_summary_provider(valobj, internal_dict):
    x_val = float(valobj.GetChildMemberWithName("x").value)
    y_val = float(valobj.GetChildMemberWithName("y").value)

    return f"({x_val:.2f}, {y_val:.2f})"


def __lldb_init_module(debugger, internal_dict):
    ci = debugger.HandleCommand
    ci("type summary add -F formatters.vec2_summary_provider Vec2")
