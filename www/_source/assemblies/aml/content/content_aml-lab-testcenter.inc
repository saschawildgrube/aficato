<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{

			$this->GetWebsite()->IncludeModule('system/testmanager');

			$arrayModuleConfig = array();
			$arrayModuleConfig['hidefilteroptions'] = true;
			$arrayModuleConfig['filteroptions'] = array(
				array(
					'field' => 'assembly_id',
					'operator' => '=',
					'value' => 'aml')
					,
				array(
					'field' => 'site_path',
					'operator' => '=',
					'value' => 'local')
					);

			$testmanager = new CTestManagerModule(
				$this->GetWebsite(),
				'aml',
				$arrayModuleConfig);
			
			$arrayItems['MODULE_TESTMANAGER'] = $testmanager->RenderModule();
		}
	}
	
