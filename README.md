## To enable lldb custom format

Run command: `command script import ./.lldb/formatters.py`

## To make CLion auto load .lldbinit from project root, add the following line to `~/.lldbinit`

`settings set target.load-cwd-lldbinit true`