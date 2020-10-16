<?php

	require_once('aml.inc');
	require_once('aml-module.inc');
	
	class CAMLAssembly extends CAssembly	
	{
		function __construct(
			CWebSite $website,
			$strRootDir = '')
		{
			parent::__construct(
				$website,
				'aml',
				$strRootDir);
		}
		
		function CallbackWebsiteInit()
		{
			$bResult = parent::CallbackWebsiteInit();
			$this->GetWebsite()->LoadStringResources('aml');
			
			$this->SetOutputItems(
				LoadNamedArrayFromTextFile(
					$this->GetAssemblyLine()->GetLayoutConfigFilePath(
						$this->GetWebsite()->GetLayout(),
						'aml')));
			
			return $bResult;
		}
		


	}
	
	

