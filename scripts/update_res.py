#!/usr/bin/python

import os
import sys 
sys.path.append("../awtk/scripts") 
sys.path.append("../../awtk/scripts") 
 
import update_res_common as common

APP_ROOT=os.getcwd()
if APP_ROOT.endswith('scripts'):
  APP_ROOT=os.path.dirname(APP_ROOT)

os.chdir(APP_ROOT);
AWTK_ROOT=common.joinPath(APP_ROOT, '../awtk');
ASSETS_ROOT=common.joinPath(APP_ROOT, 'assets')
ASSET_C=common.joinPath(APP_ROOT, 'demos/assets.c')

common.init(AWTK_ROOT, ASSETS_ROOT, 'default', ASSET_C);

common.updateRes()
