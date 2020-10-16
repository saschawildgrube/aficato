<?php

	require_once(GetWDKDir().'wdk_json.inc');

	abstract class CReactElement extends CElement
	{
		function __construct(
			CWebSite &$website,
			$strElementID,
			$strStyleID,
			$bLayoutCFG,
			$bLayoutHTML)
		{
			parent::__construct(
				$website,
				$strElementID,
				$strStyleID,
				$bLayoutCFG,
				$bLayoutHTML);
			
			$this->AddJavaScript('wdk');
			$this->GetWebsite()->ActivateReact();
		}
	
		function AddJavaScriptReactRenderByID($strReactComponent,$strDomElementID,$arrayProps = array())
		{
			$strJson = JsonEncode($arrayProps);
			$strPropsVarName = $strDomElementID.'_props';
			$strJavaScript = $strPropsVarName.' = '.$strJson.";";
			$strJavaScript .= "ReactDOM.render(e(".$strReactComponent.",".$strPropsVarName."),document.getElementById('".$strDomElementID."'));";
			$this->AddJavaScriptSourceCode(
				$this->GetElementID().'-'.$strDomElementID,
				$strJavaScript);
		}		
		
		
		function RenderReactComponent(
			$strReactComponent,
			$arrayProps = array())
		{
			$strDomElementID = $this->GetElementID().'_'.MakeNumericToken(10);
			
			$this->AddJavaScriptReactRenderByID(
				$strReactComponent,
				$strDomElementID,
				$arrayProps);
				
			$strOutput = '<div id="'.$strDomElementID.'"></div>';
			return $strOutput;				
		}	

	}

