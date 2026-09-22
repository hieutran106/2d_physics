import lldb


def vec2_summary_provider(valobj, internal_dict):
    x_val = float(valobj.GetChildMemberWithName("x").value)
    y_val = float(valobj.GetChildMemberWithName("y").value)

    return f"({x_val:.2f}, {y_val:.2f})"


def shape_type_provider(shape_ptr):
    # shape_ptr is a Shape* (polymorphic base). Resolve the runtime type
    # via the dynamic value, then map the concrete class name to a label.
    if not shape_ptr.IsValid() or shape_ptr.GetValueAsUnsigned() == 0:
        return "None"
    dyn = shape_ptr.GetDynamicValue(lldb.eDynamicCanRunTarget)
    type_name = dyn.GetTypeName()  # e.g. "CircleShape *"
    if "Box" in type_name:
        return "Box"
    if "Circle" in type_name:
        return "Circle"
    if "Polygon" in type_name:
        return "Polygon"
    return type_name


def body_summary_povider(valobj, internal_dict):
    pos = valobj.GetChildMemberWithName("position")
    vel = valobj.GetChildMemberWithName("velocity")

    pos_str = vec2_summary_provider(pos, internal_dict)
    vel_str = vec2_summary_provider(vel, internal_dict)
    shape_str = shape_type_provider(valobj.GetChildMemberWithName("shape"))

    mass = float(valobj.GetChildMemberWithName("mass").value)
    mass_str = f"mass={mass:.2f}" if mass != 0 else "STATIC"
    summary = f"ht shape={shape_str} pos={pos_str} vel={vel_str} {mass_str}"
    return summary


def __lldb_init_module(debugger, internal_dict):
    ci = debugger.HandleCommand
    ci("type summary add -F formatters.vec2_summary_provider Vec2")
    ci("type summary add -F formatters.body_summary_povider Body")
