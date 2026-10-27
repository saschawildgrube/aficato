<?php

	require_once(GetWDKDir().'wdk_module.inc');

	class CJsonEditorModule extends CModule
	{
		function __construct(CWebSite $website, $strInstanceID = '', $arrayConfig = array())
		{
			parent::__construct($website, 'devtools/jsoneditor', $strInstanceID, $arrayConfig);
		}

		function OnRenderModule($strState)
		{
			$this->IncludeElement('jsoneditor');
			$editor = new CJsonEditorElement($this->GetWebsite(), 'default');
			$this->SetOutputItem('EDITOR', $editor->RenderJsonEditor());
			return 'start';
		}
	}
