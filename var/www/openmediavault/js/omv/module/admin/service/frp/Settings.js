Ext.define('OMV.module.admin.service.frp.Settings', {
    extend: 'OMV.workspace.form.Panel',
    
    // This path tells which RPC module and methods this panel will call to get 
    // and fetch its form values.
    rpcService: 'frp',
    rpcGetMethod: 'getSettings',
    rpcSetMethod: 'setSettings',
    
    // getFormItems is a method which is automatically called in the 
    // instantiation of the panel. This method returns all fields for 
    // the panel.
    getFormItems: function() {
        return [{
            // xtype defines the type of this entry. Some different types
            // is: fieldset, checkbox, textfield and numberfield. 
            xtype: 'fieldset',
            title: _('General'),
            fieldDefaults: {
                labelSeparator: ''
            },
            // The items array contains items inside the fieldset xtype.
            items: [{
                xtype: 'checkbox',
                // The name option is sent together with is value to RPC
                // and is also used when fetching from the RPC.
                name: 'enable',
                fieldLabel: _('Enable'),
                // checked sets the default value of a checkbox.
                checked: false
            },
			{
                xtype: 'textfield',
                name: 'server_addr',
                fieldLabel: _('服务器地址'),
				allowBlank: false
            },
			{
                xtype: 'numberfield',
                name: 'server_port',
                fieldLabel: _('端口'),
                allowDecimals: false,
                allowBlank: true
            },
			{
                xtype: 'textfield',
                name: 'protocol',
                fieldLabel: _('protocol加速方式'),
                allowDecimals: false,
                allowBlank: true
            },
			{
                xtype: 'textfield',
                name: 'privilege_token',
                fieldLabel: _('privilege_token'),
                allowDecimals: false,
                allowBlank: true
            },
			{
                xtype: 'textfield',
                name: 'custom_domains',
                fieldLabel: _('绑定域名'),
                allowDecimals: false,
                allowBlank: true
            },
			{
                xtype: 'numberfield',
                name: 'local_port',
                fieldLabel: _('本地端口'),
                minValue: 0,
                maxValue: 100,
                allowDecimals: false,
                allowBlank: true
            },
			{
                xtype: 'textfield',
                name: 'type',
                fieldLabel: _('类型'),
                allowDecimals: false,
                allowBlank: true
            },
			{
                xtype: 'textfield',
                name: 'local_ip',
                fieldLabel: _('本地IP'),
                allowDecimals: false,
                allowBlank: true
            }
			]
        },{
			xtype: 'fieldset',
            title: _('高级'),
            fieldDefaults: {
                labelSeparator: ''
            },
			items: [{
                xtype: 'checkbox',
                name: 'use_gzip',
                fieldLabel: _('启用压缩'),
                checked: false
            },
			{
                xtype: 'checkbox',
                name: 'use_encryption',
                fieldLabel: _('use_encryption'),
                checked: false
            },			
			{
                xtype: 'checkbox',
                name: 'privilege_mode',
                fieldLabel: _('privilege_mode'),
                checked: false
            },
			{
                xtype: 'numberfield',
                name: 'pool_count',
                fieldLabel: _('pool_count'),
                allowDecimals: false,
                allowBlank: true
            },
			{
			   xtype: 'fieldcontainer',
			   fieldLabel: 'Check Box Field',
			   defaultType: 'checkboxfield',
				   items: [{
					  boxLabel  : 'web',
					  inputValue: 'web',
					  id        : 'checkbox1'
				   },{
					  boxLabel  : 'SSH',
					  inputValue: 'SSH',
					  checked   : true,
					  id        : 'checkbox2'
				   },{
					  boxLabel  : 'dns',
					  inputValue: 'dns',
					  id        : 'checkbox3'
				   },{
					  boxLabel  : 'p2p_ssh',
					  inputValue: 'p2p_ssh',
					  id        : 'checkbox3'
				   },
				   {
					  boxLabel  : 'plugin_http_proxy',
					  inputValue: 'plugin_http_proxy',
					  id        : 'checkbox4'
				   }]
				},
		{
				xtype: "button",
				name: "buy_account",
				disabled: true,
				text: _("购买账号"),
				cls:'buy_account'
			}
			]
	 }];	
    }
});

// Register a panel into the GUI.
//
// path: 
//     We want to add the panel in our example node. 
//     The node was configured with the path /service and the id example.
//     The path is therefore /service/example.
//
// className: 
//     The panel which should be registered and added (refers to 
//     the class name).
OMV.WorkspaceManager.registerPanel({
    id: 'settings',
    path: '/service/frp',
    text: _('Settings'),
    position: 10,
    className: 'OMV.module.admin.service.frp.Settings'
});