import lldb


def vec2_summary_provider(valobj, internal_dict):
    x_val = float(valobj.GetChildMemberWithName("x").value)
    y_val = float(valobj.GetChildMemberWithName("y").value)

    return f"({x_val:.2f}, {y_val:.2f})"


def body_summary_povider(valobj, internal_dict):
    pos = valobj.GetChildMemberWithName("position")
    vel = valobj.GetChildMemberWithName("velocity")

    pos_str = vec2_summary_provider(pos, internal_dict)
    vel_str = vec2_summary_provider(vel, internal_dict)
    summary = f"ht pos={pos_str} vel={vel_str}"
    return summary


def __lldb_init_module(debugger, internal_dict):
    ci = debugger.HandleCommand
    ci("type summary add -F formatters.vec2_summary_provider Vec2")
    ci("type summary add -F formatters.body_summary_povider Body")
