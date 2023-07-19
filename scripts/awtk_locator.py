import os
import sys

AWTK_ROOT = '../awtk-vnc'

def getAwtkRoot():
    return AWTK_ROOT

def getAwtkSDKPath():
    env = os.environ
    if 'AWTK_SDK_PATH' in env:
        return env['AWTK_SDK_PATH']
    else:
        return ''

def getAwtkOrAwtkLinuxFbRoot(is_linux_fb):
    if is_linux_fb:
        return locateAWTK('awtk-linux-fb')
    else:
        return locateAWTK('awtk')

def locateAWTK(awtk):
    awtk_root = ''

    if not os.path.exists(awtk_root):
        dirnames = ['../'+awtk, '../../'+awtk, '../../../'+awtk]
        for dirname in dirnames:
            if os.path.exists(dirname):
                awtk_root = dirname
                break

    if not os.path.exists(awtk_root):
        awtk_sdk_path = getAwtkSDKPath();
        if os.path.exists(awtk_sdk_path):
            awtk_root = awtk_sdk_path + '/' + awtk

    return os.path.abspath(awtk_root)

def init(ARGUMENTS = None):
    global AWTK_ROOT
    global LINUX_FB

    if ARGUMENTS:
        AWTK_ROOT = ARGUMENTS.get('AWTK_ROOT', '')
        LINUX_FB = ARGUMENTS.get('LINUX_FB', '')
    else:
        LINUX_FB = ''
    
    if not os.path.exists(AWTK_ROOT):
        AWTK_ROOT = getAwtkOrAwtkLinuxFbRoot(LINUX_FB != '')
    elif os.path.exists(LINUX_FB):
        print(' do not set LINUX_FB and AWTK_ROOT !!!')
        sys.exit()

    if LINUX_FB:
        AWTK_SCRIPTS_ROOT = os.path.join(AWTK_ROOT, '../awtk/scripts')
    else:
        AWTK_SCRIPTS_ROOT = os.path.join(AWTK_ROOT, 'scripts')
    sys.path.insert(0, AWTK_SCRIPTS_ROOT)

    print('AWTK_ROOT: ' + AWTK_ROOT)
    print('AWTK_SCRIPTS_ROOT: ' + AWTK_SCRIPTS_ROOT)
