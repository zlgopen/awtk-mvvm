/**
 * @class iotjs_fs_view_model
 * @parent ViewModel
 * @annotation ["model", "view_model", "custom_prop"]
 */
ViewModel('iotjs_fs_view_model', {
  data: {
    /**
     * @property {String} file_info
     * @annotation ["readable", "writable", "defvalue:click_get_button"]
     */
    file_info: 'click_get_button'
  },
  methods: {
    /**
     * @method read_file_info
     * @annotation ["command"]
     * command
     * @param {String|Object} args 命令的参数。
     * @return {TRET} 返回RET_OK表示成功，否则表示失败。
     */
    canRead_file_info: function(args) {
      return true;
    },
    read_file_info: function(args) {
      var fs = require('fs');
      var filePath = process.cwd() + '/res/assets/default/raw/data/file_info.txt';

      if (!fs.existsSync(filePath)) {
        console.log('Not found ' + filePath);
      } else {
        fs.readFile(filePath, function(err, data) {
          this.file_info = data.toString();
          console.log('Read ' + filePath + ':' + this.file_info);
          this.notifyPropsChanged();
        }.bind(this));
  
        // this.file_info = fs.readFileSync(filePath, 'utf-8').toString();
        // console.log('Read ' + filePath + ':' + this.file_info);
        // this.notifyPropsChanged();
      }
    }
  }
});