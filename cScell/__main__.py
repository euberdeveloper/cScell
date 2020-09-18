from cScell.cScell import generate_scell
import sys

APP_NAME = sys.argv[1] if len(sys.argv) > 1 else None
JSON_COMMANDS = sys.argv[2] if len(sys.argv) > 2 else None
OUTPUT_PATH = sys.argv[3] if len(sys.argv) > 3 else None

generate_scell(APP_NAME, JSON_COMMANDS, OUTPUT_PATH)