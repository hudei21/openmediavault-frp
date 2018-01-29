<?php

require_once 'openmediavault/config.inc';
require_once 'openmediavault/error.inc';
require_once 'openmediavault/initscript.inc';
require_once 'openmediavault/module.inc';

class OMVModuleFrp extends OMVModuleServiceAbstract implements OMVINotifyListener, OMVIModuleServiceStatus, OMVIModuleZeroconf{
	/**
	 * 事件消息路径
	 *
	 * @var 字符型
	 */
	private $eventMessagePath = 'org.openmediavault.services.frp';
	
	
	/**
	 * 获取服务的配置XPath
	 *
	 * @return 字符型
	 */
	private function getXpath(){
		return '/config/services/frp';
	}

	/**
	 * 模块名称
	 */
	public function getName(){
		return 'frp';
	}
	
	/**
	 * 服务停止
	 */
	public function stopService() {
		$initd = new OMVSysVInitScript("frpc", FALSE);
		$initd->exec();
	}

	/**
	 * 服务启动
	 * @throw E_CONFIG_GET_OBJECT_FAILED
	*/
	public function startService() {
		global $xmlConfig;
		$xpath = "//services/frp";
		$object = $xmlConfig->get($xpath);
		if (is_null($object)) {
			throw new OMVException(OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED,
			  $xpath);
		}
		if (FALSE === boolvalEx($object['enable']))
			return;
		$initd = new OMVSysVInitScript("frpc", TRUE);
		$initd->exec();
	}

	 
	/**
	 * 模块状态
	 *
	 * @return 数组型
	 *
	 * @throws OMVException
	 */
	public function getStatus(){
		global $xmlConfig;

		// 获取对象配置
		$object = $xmlConfig->get($this->getXpath());

		if (is_null($object)) {
			throw new OMVException(
				OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED,
				$this->getXpath()
			);
		}
		$initd = new OMVSysVInitScript("frpc");
		// 返回服务状态,该信息显示在诊断/服务中。
		return array(
			'name' => $this->getName(),
			'title' => gettext('Frpc'),
			'enabled' => boolval($object['enable']),
			"running" => ($initd->status() == 0)
		);
	}
	

	
	/**
	 * 生成配置
	 *
	 * @return void
	 *
	 * @throws OMVException
	 */
	public function applyConfig(){
		global $xmlConfig;
		$cmd = sprintf('export LANG=C; omv-mkconf %s 2>&1', $this->getName());

		if (0 !== $this->exec($cmd, $output)) {
			throw new OMVException(
				OMVErrorMsg::E_EXEC_FAILED,
				$cmd,
				implode(PHP_EOL, $output)
			);
		}
	}

	/**
	 * Get the Zeroconf service configuration.
	 */
	public function getZeroconfConfig() {
		return array(
			  "id" => "frp",
			  "title" => gettext("Frpc")
		  );
	}
	

	/**
	 * 绑定监听器
	 *
	 * @param OMVNotifyDispatcher $dispatcher
	 * @return void
	 */
	public function bindListeners(OMVNotifyDispatcher $dispatcher){	
		// 添加侦听器。最常见的事情是监视服务的配置更改。
		// 当配置被更改时，模块设置为dirty(如下所示)。将一个模块设置为dirty，
		// 使apply按钮出现在web界面中(这反过来又调用带有状态的每个模块上的applyConfig函数)。
		$dispatcher->addListener(
		  OMV_NOTIFY_MODIFY,
		  "org.openmediavault.services.frp",
		  array($this, "setDirty"));
		
		$moduleMgr = &OMVModuleMgr::getInstance();
		$dispatcher->addListener(
			OMV_NOTIFY_MODIFY,
			$this->eventMessagePath,
			[$this, 'setDirty']
		);
	}
}

// 注册模块
$moduleMgr = &OMVModuleMgr::getInstance();
$moduleMgr->registerModule(new OMVModuleFrp());