<?php

require_once 'openmediavault/config.inc';
require_once 'openmediavault/error.inc';
require_once 'openmediavault/notify.inc';
require_once 'openmediavault/rpcservice.inc';

class OMVRpcServiceFrp extends OMVRpcServiceAbstract{
	/**
	 * The main event message path.
	 *
	 * @var string
	 */
	private $eventMessagePath = 'org.openmediavault.services.frp';
	
	/**
	 * Get the base XPath of the service. This is a helper function to avoid 
	 * "magic numbers".
	 *
	 * @return string
	 */
	private function getXpath(){
		return '/config/services/frp';
	}
	
	/**
	 * Get the name of the RPC service.
	 *
	 * @return string
	 */
	public function getName(){
		return 'frp';
	}

	/**
	 * Initialize the RPC service. The RPC methods are registered in this
	 * function with $this->registerMethod.
	 *
	 * @return void
	 */
	public function initialize(){
		$this->registerMethod('getSettings');
		$this->registerMethod('setSettings');
	}

	public function getSettings($params, $context){
		// $xmlConfig is needed when reading and writing from the configuration.
		global $xmlConfig;
		
		// Validate the RPC caller context.
		//
		// validateMethodContext takes the currentcontext as the first
		// parameter. The second paramter is the valid context and that can be
		// OMV_ROLE_ADMINISTRATOR, OMV_ROLE_USER or OMV_ROLE_EVERYONE.
		// This is used to make sure that the right user accesses the method.
		$this->validateMethodContext($context, ['role' => OMV_ROLE_ADMINISTRATOR]);
		
		// Get the configuration object.
		$object = $xmlConfig->get($this->getXpath());
		
		// If no data was found, throw an exception and provide the XPath that
		// failed.
		if (is_null($object)) {
			throw new OMVException(
				OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED,
				$this->getXpath()
			);
		}
		
		// Modify the result data.
		// boolval and intval converts strings and numbers to their boolean
		// and integer value.
		$object['enable'] = boolval($object['enable']);		
		$object['server_port'] = intval($object['server_port']);		
		$object['local_port'] = intval($object['local_port']);
		$object['use_gzip'] = boolval($object['use_gzip']);
		$object['use_encryption'] = boolval($object['use_encryption']);
		$object['privilege_mode'] = boolval($object['privilege_mode']);
		$object['pool_count'] = intval($object['pool_count']);		
		return $object;
	}

	public function setSettings($params, $context){
		global $xmlConfig;
		
		$this->validateMethodContext($context, array(
			"role" => OMV_ROLE_ADMINISTRATOR
		));

		// Validate the parameters of the RPC service method.
		$this->validateMethodParams($params, '{
			  "type":"object",
			  "properties":{
					"enable":{"type":"boolean"},
					"server_addr":{"type":"string"},
					"protocol":{"type":"string"},
					"privilege_token":{"type":"string"},
					"custom_domains":{"type":"string"},
					"type":{"type":"string"},
					"local_ip":{"type":"string"},
					"server_port":{"type":"integer"},
					"local_port":{"type":"integer"},
					"use_gzip":{"type": "boolean"},
					"use_encryption":{"type": "boolean"},
					"privilege_mode":{"type": "boolean"},
					"pool_count":{"type":"integer"}
				}
			}');

		// Update the configuration object.
		$object = [
			'enable' => boolval($params['enable']),
			'server_addr' => $params['server_addr'],
			'protocol' => $params['protocol'],
			'privilege_token' => $params['privilege_token'],
			'custom_domains' => $params['custom_domains'],
			'type' => $params['type'],
			'local_ip' => $params['local_ip'],
			'server_port' => $params['server_port'],
			'local_port' => $params['local_port'],
			'use_gzip' => boolval($params['use_gzip']),
			'use_encryption' => boolval($params['use_encryption']),
			'privilege_mode' => boolval($params['privilege_mode']),
			'pool_count' => $params['pool_count']
		];
		
		// Update the configuration file. If it fails it throws an exception.
		if ($xmlConfig->replace($this->getXpath(), $object) === false) {
			throw new OMVException(
				OMVErrorMsg::E_CONFIG_SET_OBJECT_FAILED,
				$this->getXpath()
			);
		}
		
		// Notify configuration changes.
		//
		// This will notify event listeners such as the service module
		// to perform certain tasks. The most common one is to mark the
		// service as dirty.
		$dispatcher = &OMVNotifyDispatcher::getInstance();
		$dispatcher->notify(OMV_NOTIFY_MODIFY, $this->eventMessagePath, $object);

		return $object;
	}
}

// Register the RPC service.
$rpcServiceMgr = &OMVRpcServiceMgr::getInstance();
$rpcServiceMgr->registerService(new OMVRpcServiceFrp());