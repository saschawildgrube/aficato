<?php

	class CMyContent extends CContent
	{
		
		function OnRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->SetFramework('fullwidth');
 		
			$strFilePath = $this->GetDataFilePath('mandates/tour/accounts/tour.csv');  
			$arrayAccount = AML_ReadFileAccount($strFilePath);
			
			$this->IncludeModule('aml/accountanalysis');

			$arrayConfig = array();
			$arrayConfig['demo'] = true;			
				
			$accountAnalysisModule = new CAccountAnalysisModule($this->GetWebsite(),'tour1',$arrayConfig);
			$accountAnalysisModule->SetAccount($arrayAccount);
			$accountAnalysisModule->SetState('chart');
			$arrayItems['ACCOUNTANALYSIS'] = $accountAnalysisModule->RenderModule();

			$link = new CLinkElement($this->GetWebsite(),'calltoaction');
			$arrayItems['NEXT'] = $link->RenderLink($this->GetWebsite()->MakeContentURL('tour2'),'?TID_BUTTON_NEXT?');
			
		}
	}
	
