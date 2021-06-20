<?php

	class CToolbarDefinition
	{
		private $m_arrayData;
		
		function __construct()
		{
			$this->m_arrayData = array();	
			$this->m_arrayData['tools'] = array();
		}
		
		function GetData()
		{
			return $this->m_arrayData;	
		}
		
		function AddTool(
			$strLink,
			$strIconID,
			$strText,
			$bExternal = false)
		{
			$arrayTool = array();
			$arrayTool['link'] = $strLink;
			$arrayTool['iconid'] = $strIconID;
			$arrayTool['text'] = $strText;
			$arrayTool['external'] = $bExternal;
			array_push($this->m_arrayData['tools'],$arrayTool);
		}
		
		function GetToolCount()
		{
			return ArrayCount(ArrayGetValue($this->m_arrayData,'tools'));
		}
		
		
		
		function IsValid()
		{
			$arrayTools = ArrayGetValue($this->m_arrayData,'tools');
			if (!is_array($arrayTools))
			{
				return false;	
			}
		
			foreach ($arrayTools as $arrayTool)
			{
				$strLink = ArrayGetValue($arrayTool,'link');
/*				if ($strLink == '')
				{
					return false;	
				}
				*/
				$strIconID = ArrayGetValue($arrayTool,'iconid');
				$strText = ArrayGetValue($arrayTool,'text');
				if ($strIconID == '' && $strText == '')
				{
					return false;	
				}
			}
			return true;
		}
		
	}
	
	class CToolbarElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'toolbar',
				$strStyleID,
				true,
				false);
				
		}
		

		function RenderToolbar($toolbarDef)
		{
			$this->Trace('RenderToolbar');
			
			if (!is_object($toolbarDef))
			{
				$this->Error('toolbarDef is not an object.');	
			}
			
			if ($toolbarDef->IsValid() != true)
			{
				$this->Error('toolbarDef->IsValid != true.');	
			}
			
			$strOutput = '';
			
			$strOutput .= $this->GetLayoutDef('LAYOUT_TOOLBAR_BEGIN') . "\n";
			
			$arrayTools = ArrayGetValue($toolbarDef->GetData(),'tools');
			$nCount = ArrayCount($arrayTools);
			$nCurrent = 0;
			 
			foreach ($arrayTools as $arrayTool)
			{
				$nCurrent++;
				
				$bExternal = GetBoolValue(ArrayGetValue($arrayTool,'external'));
				$strExternal = '';
				if ($bExternal == true)
				{
					$strExternal = '_EXTERNAL';
				}
				
				$strOutputTool = $this->GetLayoutDef('LAYOUT_TOOLBAR_TOOL'.$strExternal);
				
				$strLink = ArrayGetValue($arrayTool,'link');
				$strIconID = ArrayGetValue($arrayTool,'iconid');
				$strText = ArrayGetValue($arrayTool,'text');
				
				if ($strIconID != '')
				{
					$strIconStyle = $this->GetLayoutDef('LAYOUT_TOOLBAR_ICON_STYLE');
					if ($strIconStyle == '')
					{
						$strIconStyle = 'default';
					}
					$this->GetWebsite()->IncludeElement('icon');
					$icon = new CIconElement(
						$this->GetWebsite(),
						$strIconStyle);
					$strOutputIcon = $icon->RenderIcon($strIconID);
				}
				else
				{
					$strOutputIcon = '';	
				}
				
				$strOutput .= ReplaceTagsWDK($strOutputTool,
					array(
						'ICON' => $strOutputIcon,
						'TEXT' => $strText,
						'URL' => $strLink
						)
					);
				
				if ($nCurrent < $nCount)
				{
					$strOutput .= $this->GetLayoutDef('LAYOUT_TOOLBAR_SEPARATOR') . "\n"; 
				}
			}
			
			$strOutput .= $this->GetLayoutDef('LAYOUT_TOOLBAR_END') . "\n";
			
			return $strOutput;
		}
		
	}





