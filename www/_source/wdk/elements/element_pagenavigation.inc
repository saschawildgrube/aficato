<?php

	class CPageNavigationElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'pagenavigation',
				$strStyleID,
				true,
				true);
		}
		
		function RenderPageNavigation($nCurrentOffset,$nCount,$strOffsetParam,$nPageBlock = 0)
		{
			$nCurrentOffset = intval($nCurrentOffset);
			$nCount = intval($nCount);
			$nPageBlock = intval($nPageBlock);
			
			if ($nCount == 0)  
			{
				return '';	
			}
			
			$strOutput = $this->GetLayoutHTML(); 
			
			$arrayItems = array();
		
			$strLinkStyle = $this->GetLayoutDef('LAYOUT_PAGENAVIGATION_LINK_STYLE');
			if ($strLinkStyle == '')
			{
				$strLinkStyle = 'default';
			}
			$strLinkStylePages = $this->GetLayoutDef('LAYOUT_PAGENAVIGATION_LINK_STYLE_PAGES');
			if ($strLinkStylePages == '')
			{
				$strLinkStylePages = $strLinkStyle;
			}

			$strLinkStyleCurrent = $this->GetLayoutDef('LAYOUT_PAGENAVIGATION_LINK_STYLE_CURRENT');
			if ($strLinkStyleCurrent == '')
			{
				$strLinkStyleCurrent = $strLinkStyle;
			}
		
			$link = new CLinkElement($this->GetWebsite(),$strLinkStyle);
			$linkPages = new CLinkElement($this->GetWebsite(),$strLinkStylePages);
			$linkCurrent = new CLinkElement($this->GetWebsite(),$strLinkStyleCurrent);
			
			$nMaxPageBlock = intval($this->GetLayoutDef('LAYOUT_PAGENAVIGATION_MAX_PAGEBLOCK'));
			if ($nMaxPageBlock > 0 && ($nPageBlock != 0))
			{
				if ($nPageBlock > $nMaxPageBlock)
				{
					$nPageBlock = $nMaxPageBlock;	
				}
			}
			
			
			$arrayParams = array();
			
			if ($nCurrentOffset >= $nCount)
			{
				$nCurrentOffset = $nCount - 1;	
			}
			
			if ($nCurrentOffset < 0)
			{
				$nCurrentOffset = 0;	
			}
			
			if (($nPageBlock != 0) && ($nCurrentOffset+1 > $nPageBlock))
			{
				$arrayParams[$strOffsetParam] = max(0,$nCurrentOffset - $nPageBlock);
				$strURL = $this->GetWebsite()->MakeURL($arrayParams);
				$arrayItems['PN_PREVIOUSBLOCK'] = $link->RenderLink($strURL,$this->GetLayoutDef('LAYOUT_PAGENAVIGATION_PREVIOUSBLOCK'));
			}
			else
			{
				$arrayItems['PN_PREVIOUSBLOCK'] = '';				
			}
			
			if ($nCurrentOffset != 0)
			{
				$arrayParams[$strOffsetParam] = 0;
				$strURL = $this->GetWebsite()->MakeURL($arrayParams);
				$arrayItems['PN_FIRST'] = $link->RenderLink($strURL,$this->GetLayoutDef('LAYOUT_PAGENAVIGATION_FIRST'));
	
				$arrayParams[$strOffsetParam] = max(0,$nCurrentOffset - 1);
				$strURL = $this->GetWebsite()->MakeURL($arrayParams);
				$arrayItems['PN_PREVIOUS'] = $link->RenderLink($strURL,$this->GetLayoutDef('LAYOUT_PAGENAVIGATION_PREVIOUS'));
		
			}
			else
			{
				$arrayItems['PN_FIRST'] = '';
				$arrayItems['PN_PREVIOUS'] = '';
			}
			
			$strPages = '';
			
			if ($nPageBlock == 0)
			{
				$nLowerBoundary = 0;
				$nUpperBoundary = $nCount;
			}
			else
			{
				$nLowerBoundary = floor($nCurrentOffset / $nPageBlock) * $nPageBlock;
				$nUpperBoundary = min($nCount,$nLowerBoundary+$nPageBlock);
			}
			
			if ($nCount > 1)
			{
				for ($nIndex = $nLowerBoundary; $nIndex < $nUpperBoundary; $nIndex++)
				{
					$arrayParams[$strOffsetParam] = $nIndex;
					$strURL = $this->GetWebsite()->MakeURL($arrayParams);
					if ($nCurrentOffset == $nIndex)
					{
						$strLayoutDef = $this->GetLayoutDef('LAYOUT_PAGENAVIGATION_PAGE_CURRENT');
					}
					else
					{
						$strLayoutDef = $this->GetLayoutDef('LAYOUT_PAGENAVIGATION_PAGE');
					}
					
					$strText = ReplaceTagsWDK(
							$strLayoutDef,
							array('PAGE' => ''.($nIndex+1))
							);
					if ($nCurrentOffset == $nIndex)
					{
						$strPages .= $linkCurrent->RenderLink($strURL,$strText);
					}
					else
					{
						$strPages .= $linkPages->RenderLink($strURL,$strText);
					}
				}
			}
			
			$arrayItems['PN_PAGES'] = $strPages;



			if (($nPageBlock != 0) && ($nCurrentOffset+$nPageBlock <= $nCount-1) && ($nCount > 0))
			{
				$arrayParams[$strOffsetParam] = min($nCount-1,$nCurrentOffset + $nPageBlock);
				$strURL = $this->GetWebsite()->MakeURL($arrayParams);
				$arrayItems['PN_NEXTBLOCK'] = $link->RenderLink($strURL,$this->GetLayoutDef('LAYOUT_PAGENAVIGATION_NEXTBLOCK'));
			}
			else
			{
				$arrayItems['PN_NEXTBLOCK'] = '';				
			}
			
			if (($nCurrentOffset != $nCount-1) && ($nCount > 0))
			{
				$arrayParams[$strOffsetParam] = min($nCount-1,$nCurrentOffset + 1);
				$strURL = $this->GetWebsite()->MakeURL($arrayParams);
				$arrayItems['PN_NEXT'] = $link->RenderLink($strURL,$this->GetLayoutDef('LAYOUT_PAGENAVIGATION_NEXT'));
	
				$arrayParams[$strOffsetParam] = $nCount-1;
				$strURL = $this->GetWebsite()->MakeURL($arrayParams);
				$arrayItems['PN_LAST'] = $link->RenderLink($strURL,$this->GetLayoutDef('LAYOUT_PAGENAVIGATION_LAST'));
			}
			else
			{
				$arrayItems['PN_NEXT'] = '';
				$arrayItems['PN_LAST'] = '';
			}
			
			
			//$arrayItems['TEXT'] = $strText;
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			

		}


	}

