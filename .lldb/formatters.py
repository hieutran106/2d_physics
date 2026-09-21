"""LLDB data formatters for the 2d_physics project.

Runs under LLDB's bundled Python (3.9.6 on this machine). Only uses the
built-in `lldb` module, so the interpreter version does not matter here.

Loaded via ~/.lldbinit:
    command script import /Volumes/ht-data/repo/learn/2d_physics/.lldb/formatters.py
"""

import struct

import lldb


def _f(valobj):
    """Read an SBValue holding a 32-bit float as a Python float.

    This LLDB build's SBValue has no GetValueAsFloat(); reinterpret the
    raw unsigned bits as an IEEE-754 single instead.
    """
    return struct.unpack("<f", struct.pack("<I", valobj.GetValueAsUnsigned() & 0xFFFFFFFF))[0]


# --- Vec2: summary shows components + magnitude -----------------------------
def vec2_summary(valobj, internal_dict):
    x = _f(valobj.GetChildMemberWithName("x"))
    y = _f(valobj.GetChildMemberWithName("y"))
    return f"({x:.3f}, {y:.3f})  |{(x * x + y * y) ** 0.5:.3f}|"


# --- Body: one-line summary of the interesting scalars ----------------------
def body_summary(valobj, internal_dict):
    pos = valobj.GetChildMemberWithName("position")
    px = _f(pos.GetChildMemberWithName("x"))
    py = _f(pos.GetChildMemberWithName("y"))
    mass = _f(valobj.GetChildMemberWithName("mass"))
    rot = _f(valobj.GetChildMemberWithName("rotation"))
    colliding = valobj.GetChildMemberWithName("isColliding").GetValueAsUnsigned() != 0
    kind = "static" if mass == 0.0 else "dynamic"
    hit = " HIT" if colliding else ""
    return f"pos=({px:.2f}, {py:.2f}) rot={rot:.2f} mass={mass:.2f} [{kind}]{hit}"


def __lldb_init_module(debugger, internal_dict):
    ci = debugger.HandleCommand
    ci("type summary add -F formatters.vec2_summary Vec2")
    ci("type summary add -F formatters.body_summary Body")
    # std::vector<Vec2> already gets expandable children from the libc++
    # synthetic provider; the Vec2 summary above renders each element.
