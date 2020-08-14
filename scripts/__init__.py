import os
import sys
APP_SCRIPTS_ROOT = os.path.abspath(os.path.dirname(__file__))

print('APP_SCRIPTS_ROOT:' + APP_SCRIPTS_ROOT)

sys.path.insert(0, APP_SCRIPTS_ROOT)
