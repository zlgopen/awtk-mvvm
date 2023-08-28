import os
import sys

APP_SCRIPTS_ROOT = os.path.abspath(os.path.dirname(__file__))
sys.path.insert(0, APP_SCRIPTS_ROOT)
import awtk_locator as locator

def update_res(ARGUMENTS):
    locator.init(ARGUMENTS)

    import update_res_app as updater
    updater.run(locator.getAwtkRoot(), None, True)

awtk_root = locator.get_awtk_root_by_args(sys.argv[1:])
ARGUMENTS = dict()
ARGUMENTS['AWTK_ROOT'] = awtk_root
update_res(ARGUMENTS)

sys.path.remove(APP_SCRIPTS_ROOT)