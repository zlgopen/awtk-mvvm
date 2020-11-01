import os
import sys

AWTK_ROOT = ''

def getAwtkRoot():
    return AWTK_ROOT

def getAwtkDesignerPath():
    env = os.environ
    if 'AWTK_DESIGNER_PATH' in env:
        return env['AWTK_DESIGNER_PATH']
    else:
        return ''

def getAwtkOrAwtkLinuxFbRoot(LINUX_FB):
    if LINUX_FB:
        return locateAWTK('awtk-linux-fb')
    else:
        return locateAWTK('awtk')

def locateAWTK(awtk):
    awtk_root = ''

    designer_path = getAwtkDesignerPath();
    if os.path.exists(designer_path):
        awtk_root = designer_path + '/SDK/' + awtk

    if not os.path.exists(awtk_root):
        dirnames = ['../'+awtk, '../../'+awtk, '../../../'+awtk]
        for dirname in dirnames:
            if os.path.exists(dirname):
                awtk_root = dirname
                break

    return os.path.abspath(awtk_root)

def init(ARGUMENTS = None):
    global AWTK_ROOT
    LINUX_FB = ''

    if ARGUMENTS:
        AWTK_ROOT = ARGUMENTS.get('AWTK_ROOT', '')
        LINUX_FB = ARGUMENTS.get('LINUX_FB', '') != ''

    if not os.path.exists(AWTK_ROOT):
        AWTK_ROOT = getAwtkOrAwtkLinuxFbRoot(LINUX_FB)

    if LINUX_FB:
        AWTK_SCRIPTS_ROOT = os.path.join(AWTK_ROOT, '../awtk/scripts')
    else:
        AWTK_SCRIPTS_ROOT = os.path.join(AWTK_ROOT, 'scripts')
    sys.path.insert(0, AWTK_SCRIPTS_ROOT)

    print('AWTK_ROOT: ' + AWTK_ROOT)
    print('AWTK_SCRIPTS_ROOT: ' + AWTK_SCRIPTS_ROOT)
