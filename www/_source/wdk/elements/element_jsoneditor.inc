<?php

	require_once(GetWDKDir().'wdk_element_react.inc');

	class CJsonEditorElement extends CReactElement
	{
		function __construct(CWebSite $website, $strStyleID = 'default')
		{
			parent::__construct($website, 'jsoneditor', $strStyleID, false, false);
			$this->AddJavaScript('react-jsoneditor');
			$this->AddStyleSheet('jsoneditor');
			$this->GetWebsite()->ActivateFontAwesome4();
		}

		function RenderJsonEditor()
		{
			$strID = 'jsoneditor_'.MakeNumericToken(10);
			$this->AddJavaScriptReactRenderByID('JsonEditor', $strID);
			return '<div id="'.$strID.'" class="wdk-jsoneditor"></div>';
		}
	}
