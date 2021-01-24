<?php

	require_once(GetAssemblyDir('aml').'aml.inc');
	require_once(GetAssemblyDir('aml').'aml-module.inc');
	
	class CAMLPlugin extends CWebsitePlugin	
	{
		function __construct(
			CWebSite $website)
		{
			parent::__construct(
				$website,
				'aml');
		}
		
		function OnInit()
		{
			$bResult = parent::OnInit();
			$this->GetWebsite()->LoadStringResources('aml');
			
			$this->GetWebsite()->SetLayoutConfigItems(
				LoadNamedArrayFromTextFile(
					$this->GetWebsite()->GetAssemblyLine()->GetLayoutConfigFilePath(
						$this->GetWebsite()->GetLayout(),  
						'aml')));
			
			return $bResult;
		}
		


	}
	
	

