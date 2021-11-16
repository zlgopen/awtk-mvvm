ViewModel('temperature_ex', {
  data: {
    value: 20,
    applydValue: 20
  },
  methods: {
    canApply: function(args) {
      return this.applydValue != this.value;
    },
    apply: function(args) {
      this.applydValue = this.value;
      this.notifyPropsChanged();
    },
    canChangeLocale: function() {
      return true;
    },
    changeLocale: function() {
      locale = getLocale();
      if (locale.language == 'zh') {
        setLocale('en', 'US');
      } else {
        setLocale('zh', 'CN');
      }
    },
    canChangeTheme: function() {
      return true;
    },
    changeTheme: function() {
      theme = getTheme();
      if (theme == 'default') {
        setTheme('dark');
      } else {
        setTheme('default');
      }
    }
  },
  onWillMount: function(req) {
    this.value = req.value || 20;
  }
});
