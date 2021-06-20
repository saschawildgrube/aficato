<?php

	class CTabNavigationDefinition
	{
		private $m_arrayData;
		
		function __construct()
		{
			$this->m_arrayData = array();	
			$this->m_arrayData["tabs"] = array();
		}
		
		function GetData()
		{
			return $this->m_arrayData;	
		}
		
		function AddTab($strTabID, $strText, $strURL)
		{
			$arrayTab = array();
			$arrayTab["id"] = $strTabID;
			$arrayTab["url"] = $strURL;
			$arrayTab["text"] = $strText;
			array_push($this->m_arrayData["tabs"],$arrayTab);
		}
		
		
		
		function IsValid()
		{
			$arrayTabs = ArrayGetValue($this->m_arrayData,"tabs");
			if (!is_array($arrayTabs))
			{
				return false;	
			}
			if (ArrayCount($arrayTabs) < 1)
			{
				return false;	
			}
			
			$arrayIDs = array();
		
			foreach ($arrayTabs as $arrayTab)
			{
				$strTabID = ArrayGetValue($arrayTab,"id");
				if ($strTabID == "")
				{
					return false;	
				}
				array_push($arrayIDs,$strTabID);
				$strURL = ArrayGetValue($arrayTab,"url");
				if ($strURL == "")
				{
					return false;	
				}
				$strText = ArrayGetValue($arrayTab,"text");
				if ($strText == "")
				{
					return false;	
				}
			}
			if (ArrayCount($arrayIDs) != ArrayCount(array_unique($arrayIDs)))
			{
				return false;	
			}
			return true;
		}
		
	}
	
	class CTabNavigationElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"tabnavigation",
				$strStyleID,
				true,
				false);
				
		}
		

		function RenderTabNavigation($tabnavigationDef,$strActiveTabID)
		{
			$this->Trace("RenderTabNavigation");
			
			if (!is_object($tabnavigationDef))
			{
				$this->Error("tabnavigationDef is not an object.");	
			}
			
			if ($tabnavigationDef->IsValid() != true)
			{
				$this->Error("tabnavigationDef->IsValid != true.");	
			}

			$this->IncludeElement("format");
			
			$strOutput = "";
			
			$strOutput .= $this->GetLayoutDef("LAYOUT_TABNAVIGATION_BEGIN") . "\n";
			

			
			$arrayTabs = ArrayGetValue($tabnavigationDef->GetData(),"tabs");
			$nCount = ArrayCount($arrayTabs);
			$nCurrent = 0;
			
			$bPreviousActive = false;
			$bActive = false;
			
			
			$linkinactive = null;
			$strStyle = $this->GetLayoutDef("LAYOUT_TABNAVIGATION_STYLE_INACTIVE");
			if ($strStyle != "")
			{
				$linkinactive = new CLinkElement($this->GetWebsite(),$strStyle);
			}
			else
			{
				$linkinactive = new CLinkElement($this->GetWebsite(),"default");						
			}
			
			foreach ($arrayTabs as $arrayTab)
			{
				$bPreviousActive = $bActive;
				$nCurrent++;
				
				if (ArrayGetValue($arrayTab,"id") == $strActiveTabID)
				{
					$bActive = true;	
				}
				else
				{
					$bActive = false;	
				}
				$strText = ArrayGetValue($arrayTab,"text");
				
				
				
				
				$strCaption = "";
				// Build the caption
				if ($bActive == true)
				{
					 // no link in case of the active tab
					$strStyle = $this->GetLayoutDef("LAYOUT_TABNAVIGATION_STYLE_ACTIVE");
					if ($strStyle != "")
					{
						$format = new CFormatElement($this->GetWebsite(),$strStyle);
						$strCaption = $format->RenderFormat($strText);
					}
					else
					{
						$strCaption = $strText;						
					}
				}
				else
				{
					$strURL = ArrayGetValue($arrayTab,"url");
					$strCaption = $linkinactive->RenderLink($strURL,$strText);
				}
				
				
								
				// left side
				if ($nCurrent == 1) // the first one
				{
					if ($bActive == true)
					{
						$strOutput .= $this->GetLayoutDef("LAYOUT_TABNAVIGATION_FIRST_ACTIVE") . "\n";
					}
					else
					{
						$strOutput .= $this->GetLayoutDef("LAYOUT_TABNAVIGATION_FIRST_INACTIVE") . "\n";
					}
					
				}
				else
				{
					if ($bActive == true)
					{
						$strOutput .= $this->GetLayoutDef("LAYOUT_TABNAVIGATION_INACTIVE_TO_ACTIVE") . "\n";
					}
					else
					{
						if ($bPreviousActive == true)
						{
							$strOutput .= $this->GetLayoutDef("LAYOUT_TABNAVIGATION_ACTIVE_TO_INACTIVE") . "\n";
						}
						else
						{
							$strOutput .= $this->GetLayoutDef("LAYOUT_TABNAVIGATION_INACTIVE_TO_INACTIVE") . "\n";
						}
					}
					
				}
				
				
				// The corpus 

				$arrayItems = array("CAPTION" => $strCaption);
				if ($bActive)
				{
					$strOutput .= ReplaceTagsWDK(
						$this->GetLayoutDef("LAYOUT_TABNAVIGATION_ACTIVE"),
						$arrayItems);
				}
				else
				{
					$strOutput .= ReplaceTagsWDK(
						$this->GetLayoutDef("LAYOUT_TABNAVIGATION_INACTIVE"),
						$arrayItems);
					
				}

				
				// right side
				
				if ($nCurrent == $nCount) // the last one
				{
					if ($bActive == true)
					{
						$strOutput .= $this->GetLayoutDef("LAYOUT_TABNAVIGATION_LAST_ACTIVE") . "\n";
					}
					else
					{
						$strOutput .= $this->GetLayoutDef("LAYOUT_TABNAVIGATION_LAST_INACTIVE") . "\n";
					}
					$strOutput .= $this->GetLayoutDef("LAYOUT_TABNAVIGATION_NO_TAB") . "\n";
					
				}
				
				
						
			}
			
				
			$strOutput .= $this->GetLayoutDef("LAYOUT_TABNAVIGATION_END") . "\n";
			
			
			
			return $strOutput;
		}
		
	}





