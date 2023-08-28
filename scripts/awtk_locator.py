import os
import sys

AWTK_ROOT = ''

def getAwtkRoot():
    return AWTK_ROOT

def getAwtkSDKPath():
    env = os.environ
    if 'AWTK_SDK_PATH' in env:
        return env['AWTK_SDK_PATH']
    else:
        return ''

def is_awtk_root(path) :
    return os.path.exists(path) and os.path.exists(os.path.join(os.path.join(path, 'scripts'), 'update_res_common.py'))

def locateAWTK(awtk):
    awtk_root = ''

    if not os.path.exists(awtk_root):
        dirnames = ['../'+awtk, '../../'+awtk]
        for dirname in dirnames:
            if is_awtk_root(dirname):
                awtk_root = dirname
                break

    if not os.path.exists(awtk_root):
        awtk_sdk_path = getAwtkSDKPath();
        if os.path.exists(awtk_sdk_path):
            path = awtk_sdk_path + '/' + awtk;
            if is_awtk_root(path) :
                awtk_root = path

    return os.path.abspath(awtk_root)

def get_args(args, longsopts = []) :
    for arg in args:
        if arg.startswith('--') :
            opt_data = None
            for opt in longsopts:
                start = arg.find(opt)
                if start > 0 :
                    opt_data = arg[start + len(opt):]
                    opt_data.strip()
                    if len(opt_data) > 0 :
                        if opt_data[0] == '"' and opt_data[-1] == '"' :
                            return opt_data[1:-1]
                        else :
                            return opt_data
    return ''

def get_awtk_root_by_args(argv) :
    AWTK_ROOT_LONGSOPTS = ['awtk_root=', 'AWTK_ROOT=']
    return get_args(argv, AWTK_ROOT_LONGSOPTS)

def init(ARGUMENTS = None):
    global AWTK_ROOT

    if ARGUMENTS:
        AWTK_ROOT = ARGUMENTS.get('AWTK_ROOT', '')

    if not os.path.exists(AWTK_ROOT):
        AWTK_ROOT = locateAWTK('awtk')

    if not os.path.exists(AWTK_ROOT):
        print('Not found ' + AWTK_ROOT + ' !!!')
        sys.exit()

    AWTK_SCRIPTS_ROOT = os.path.join(AWTK_ROOT, 'scripts')

    if not os.path.exists(os.path.join(AWTK_SCRIPTS_ROOT, 'project_scripts')) :
        sys.exit("old awtk root : " + AWTK_ROOT + ", this is project must use new AWTK version !!!!!!!!!!")

    sys.path.insert(0, AWTK_SCRIPTS_ROOT)

    print('AWTK_ROOT: ' + AWTK_ROOT)
    print('AWTK_SCRIPTS_ROOT: ' + AWTK_SCRIPTS_ROOT)
