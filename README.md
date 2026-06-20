### Asset2Nani
A tool that converts UABEA json dump back to Naninovel script. Mainly written for NONAND.

### Usage
```
asset2nani [--help] [--version] [--cmd VAR] [--output VAR] filename

Positional arguments:
  filename       input file (*.json)

Optional arguments:
  -h, --help     shows help message and exits
  -v, --version  prints version information and exits
  -c, --cmd      specify nanicmd map file (*.json) [nargs=0..1] [default: "nanicmds.json"]
  -o, --output   output file (*.nani) [nargs=0..1] [default: ""]
```