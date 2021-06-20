<?php

	require_once(GetWDKDir()."wdk_navigationtree.inc");

	class CNavigationElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"navigation",
				$strStyleID,
				true,
				false);
		}		
		
		
		function RenderNavigationItem(
			CNavigationItem $navitem,
			$strContextParameterName,
			$nStartingLevel = 0)
		{
			$strLayoutTagPrefix = "LAYOUT_NAVIGATION_ITEM";
			$strLayoutTagLevel = "_LEVEL".($navitem->m_nLevel - $nStartingLevel);

			if ($navitem->m_bCurrentContext)
			{
				$strLayoutTagContext = "_ACTIVE";
			}
			else if ($navitem->m_bCurrentPath)
			{
				$strLayoutTagContext = "_ACTIVEPATH";
			}
			else if ($navitem->IsExternalLink())
			{
				$strLayoutTagContext = "_EXTERNAL";
			}
			else
			{ 
				$strLayoutTagContext = "_INACTIVE";
			}
			
			$strLayoutTagPos = "";
			if ($navitem->m_bFirstInLevel && $navitem->m_bLastInLevel)
			{
				$strLayoutTagPos = "_ALONE";
			}
			else if ($navitem->m_bFirstInLevel)
			{
				$strLayoutTagPos = "_FIRST";
			}
			else if ($navitem->m_bLastInLevel)
			{
				$strLayoutTagPos = "_LAST";
			}
			
			$strLayoutTag = $strLayoutTagPrefix . $strLayoutTagLevel . $strLayoutTagContext . $strLayoutTagPos;
			$strTemplate = $this->GetLayoutDef($strLayoutTag);

			if ($strTemplate == "")
			{
				$strLayoutTag = $strLayoutTagPrefix . $strLayoutTagLevel . $strLayoutTagContext;
				$strTemplate = $this->GetLayoutDef($strLayoutTag);
			}
			if ($strTemplate == "")
			{
				$strLayoutTag = $strLayoutTagPrefix . $strLayoutTagLevel . "_DEFAULT" . $strLayoutTagPos;
				$strTemplate = $this->GetLayoutDef($strLayoutTag);
			}
			if ($strTemplate == "")
			{
				$strLayoutTag = $strLayoutTagPrefix . $strLayoutTagLevel . "_DEFAULT";
				$strTemplate = $this->GetLayoutDef($strLayoutTag);
			}
			if ($strTemplate == "")
			{
				$strLayoutTag = $strLayoutTagPrefix . $strLayoutTagContext;
				$strTemplate = $this->GetLayoutDef($strLayoutTag);
			}
			if ($strTemplate == "")
			{
				$strLayoutTag = $strLayoutTagPrefix . "_DEFAULT";
				$strTemplate = $this->GetLayoutDef($strLayoutTag);
			}

			if ($strTemplate == "")
			{
				$this->GetWebsite()->Error("Navigation layout template $strLayoutTag is empty");
			}
			$strOutput = ReplaceString(
				$strTemplate,
				"{NAVIGATION_TEXT}",
				$navitem->m_strText);
			$strOutput = ReplaceString(
				$strOutput,
				"{NAVIGATION_LINK}",
				$navitem->GetURL(
					$this->GetWebsite(),
					$strContextParameterName,
					false,
					$this->GetWebsite()->GetLanguage()));
			return $strOutput;
		}

		
		function RenderNavigationVertical(CNavigationTree $navtree)
		{
			$strOutput = "";
			$strOutput .= $this->GetLayoutDef("LAYOUT_NAVIGATION_BEGIN") . "\n";
			$arrayNavigationItems = $navtree->GetNavigationItemArray();
			$nCount = ArrayCount($arrayNavigationItems);
			for ($nIndex = 0; $nIndex < $nCount; $nIndex++)
			{
				if ($arrayNavigationItems[$nIndex]->m_bConditionCompliance == true)
				{
					$strOutput .= $this->RenderNavigationItem(
						$arrayNavigationItems[$nIndex],
						$navtree->GetContextParameterName()) . "\n";
					if ($nIndex < $nCount-1)
					{
						$strOutput .= $this->GetLayoutDef("LAYOUT_NAVIGATION_SEPARATOR") . "\n";
					}
				}
			}
			$strOutput .= $this->GetLayoutDef("LAYOUT_NAVIGATION_END"). "\n";
			return $strOutput;
		}
		
		function RenderNavigationVerticalPath(
			CNavigationTree $navtree,
			$nStartLevel = 0,
			$nEndLevel = 100000)
		{
			if ($nStartLevel > $nEndLevel)
			{
				$this->GetWebsite()->Error("RenderNavigationPathMode: nStartLevel > nEndLevel");
			}
			
			if ($navtree->IsInitCurrentPath() == false)
			{
				$this->GetWebsite()->Error("IsInitCurrentPath() returned false ");
			}

			
			$arrayNavigationEntries = $navtree->GetNavigationItemArray();
			
			$strOutput = "";
			$strOutput .= $this->GetLayoutDef("LAYOUT_NAVIGATION_BEGIN");
			
			$nVisibleEntries = 0;
			for ($nIndex = 0; $nIndex < ArrayCount($arrayNavigationEntries); $nIndex++)
			{
				if (	($arrayNavigationEntries[$nIndex]->m_bVisibleInPathMode == TRUE)
					&& ($arrayNavigationEntries[$nIndex]->m_nLevel >= $nStartLevel)
					&& ($arrayNavigationEntries[$nIndex]->m_nLevel <= $nEndLevel)
					&& ($arrayNavigationEntries[$nIndex]->m_bConditionCompliance == true))
				{
					$nVisibleEntries++;
				}
			}

			$nRenderedEntries = 0;
			for ($nIndex = 0; $nIndex < ArrayCount($arrayNavigationEntries); $nIndex++)
			{
				if (($arrayNavigationEntries[$nIndex]->m_bVisibleInPathMode == TRUE)
					&& ($arrayNavigationEntries[$nIndex]->m_nLevel >= $nStartLevel)
					&& ($arrayNavigationEntries[$nIndex]->m_nLevel <= $nEndLevel)
					&& ($arrayNavigationEntries[$nIndex]->m_bConditionCompliance == true))
				{
					$strOutput .= $this->RenderNavigationItem(
						$arrayNavigationEntries[$nIndex],
						$navtree->GetContextParameterName(),
						$nStartLevel) . "\n";
					if ($nRenderedEntries < $nVisibleEntries-1)
					{
						$strOutput .= $this->GetLayoutDef("LAYOUT_NAVIGATION_SEPARATOR") . "\n";
					}
					$nRenderedEntries++;
				}
			}
			$strOutput .= $this->GetLayoutDef("LAYOUT_NAVIGATION_END");
			return $strOutput;
		}			

		function RenderNavigationVerticalCurrentPath(
			CNavigationTree $navtree,
			$nStartLevel = 0,
			$nEndLevel = 100000)
		{
			if ($nStartLevel > $nEndLevel)
			{
				$this->GetWebsite()->Error("RenderNavigationPathMode: nStartLevel > nEndLevel");
			}
			
			if ($navtree->IsInitCurrentPath() == false)
			{
				$this->GetWebsite()->Error("IsInitCurrentPath() returned false ");
			}

			
			$arrayNavigationEntries = $navtree->GetNavigationItemArray();
			$nDeepestLevel = $navtree->m_nDeepestLevelVisibleInPathMode;
			
			$strOutput = "";
			$strOutput .= $this->GetLayoutDef("LAYOUT_NAVIGATION_BEGIN");
			
			$nVisibleEntries = 0;
			for ($nIndex = 0; $nIndex < ArrayCount($arrayNavigationEntries); $nIndex++)
			{
				if (	($arrayNavigationEntries[$nIndex]->m_bVisibleInPathMode == true)
					&&	($arrayNavigationEntries[$nIndex]->m_nLevel >= $nStartLevel)
					&&	($arrayNavigationEntries[$nIndex]->m_nLevel <= $nEndLevel)
					&&	($arrayNavigationEntries[$nIndex]->m_bConditionCompliance == true)
					&&	($arrayNavigationEntries[$nIndex]->m_bCurrentPath == true || $arrayNavigationEntries[$nIndex]->m_nLevel == ($nDeepestLevel) )
					)
				{
					$nVisibleEntries++;
				}
			}

			$nRenderedEntries = 0;
			for ($nIndex = 0; $nIndex < ArrayCount($arrayNavigationEntries); $nIndex++)
			{
				if (	($arrayNavigationEntries[$nIndex]->m_bVisibleInPathMode == TRUE)
					&&	($arrayNavigationEntries[$nIndex]->m_nLevel >= $nStartLevel)
					&&	($arrayNavigationEntries[$nIndex]->m_nLevel <= $nEndLevel)
					&&	($arrayNavigationEntries[$nIndex]->m_bConditionCompliance == true)
					&&	($arrayNavigationEntries[$nIndex]->m_bCurrentPath == true || $arrayNavigationEntries[$nIndex]->m_nLevel == ($nDeepestLevel))
					)
				{
					$strOutput .= $this->RenderNavigationItem(
						$arrayNavigationEntries[$nIndex],
						$navtree->GetContextParameterName(),
						$nStartLevel) . "\n";
					if ($nRenderedEntries < $nVisibleEntries-1)
					{
						$strOutput .= $this->GetLayoutDef("LAYOUT_NAVIGATION_SEPARATOR") . "\n";
					}
					$nRenderedEntries++;
				}
			}
			$strOutput .= $this->GetLayoutDef("LAYOUT_NAVIGATION_END");
			return $strOutput;
		}			

	
		function RenderNavigationHorizontalPath(CNavigationTree $navtree)
		{
			if ($navtree->IsInitCurrentPath() == false)
			{
				$this->GetWebsite()->Error("IsInitCurrentPath() returned false ");
			}
			
			$strOutput = "";
			$strOutput .= $this->GetLayoutDef("LAYOUT_NAVIGATION_BEGIN");
			
			$arrayNavigationItems = $navtree->GetNavigationItemArray();
			
			$nVisibleEntries = 0;
			for ($nIndex = 0; $nIndex < ArrayCount($arrayNavigationItems); $nIndex++)
			{
				if (	($arrayNavigationItems[$nIndex]->m_nLevel == 0)
					&& ($arrayNavigationItems[$nIndex]->m_bConditionCompliance == true))
				{
					$nVisibleEntries++;
				}
			}
			
			$nMoreThreshold = intval($this->GetLayoutDef("LAYOUT_NAVIGATION_MORE_THRESHOLD"));
			$strMoreBegin = "";
			$strMoreEnd = "";
			if ($nMoreThreshold > 0)
			{
				$strMoreBegin = $this->GetLayoutDef("LAYOUT_NAVIGATION_MORE_BEGIN");
				$strMoreEnd = $this->GetLayoutDef("LAYOUT_NAVIGATION_MORE_END");
			}
			
			$bMore = false;
			$nRenderedEntries = 0;
			for ($nIndex = 0; $nIndex < ArrayCount($arrayNavigationItems); $nIndex++)
			{
				if (	($arrayNavigationItems[$nIndex]->m_nLevel == 0)
					&&	($arrayNavigationItems[$nIndex]->m_bConditionCompliance == true))
				{
					$strOutput .= $this->RenderNavigationItem(
						$arrayNavigationItems[$nIndex],
						$navtree->GetContextParameterName()) . "\n";
					if ($nRenderedEntries < $nVisibleEntries-1)
					{
						if (	$bMore == false
							&&	$nRenderedEntries == ($nMoreThreshold-1)
							&&	$nMoreThreshold > 0)
						{
							$strOutput .= $strMoreBegin . "\n";
							$bMore = true;
						}
						else
						{
							$strOutput .= $this->GetLayoutDef("LAYOUT_NAVIGATION_SEPARATOR") . "\n";
						}
					}
					else
					{
						if ($bMore == true)
						{
							$strOutput .= $strMoreEnd . "\n";
						}	
					}
					$nRenderedEntries++;
				}
			}
			$strOutput .= $this->GetLayoutDef("LAYOUT_NAVIGATION_END");
			return $strOutput;				
		}
		
		function RenderNavigationCurrentPath(
			CNavigationTree $navtree,
			$nStartLevel = 0,
			$nEndLevel = 100)
		{
			if ($navtree->IsInitCurrentPath() == false)
			{
				$this->GetWebsite()->Error("IsInitCurrentPath() returned false ");
				return "";
			}
			
			$strOutput = "";
			
			$arrayNavigationItems = $navtree->GetNavigationItemArray();
			
			$nVisibleEntries = 0;
			for ($nIndex = 0; $nIndex < ArrayCount($arrayNavigationItems); $nIndex++)
			{
				if (($arrayNavigationItems[$nIndex]->m_bCurrentPath == TRUE)
					&& ($arrayNavigationItems[$nIndex]->m_nLevel >= $nStartLevel)
					&& ($arrayNavigationItems[$nIndex]->m_nLevel <= $nEndLevel)
					&& ($arrayNavigationItems[$nIndex]->m_bConditionCompliance == true))
				{
					$nVisibleEntries++;
				}
			}
			
			if ($nVisibleEntries == 0)
			{
				return $strOutput;	
			}

			
			
			$strOutput .= $this->GetLayoutDef("LAYOUT_NAVIGATION_BEGIN");
			

			$nRenderedEntries = 0;
			for ($nIndex = 0; $nIndex < ArrayCount($arrayNavigationItems); $nIndex++)
			{
				if (($arrayNavigationItems[$nIndex]->m_bCurrentPath == TRUE)
					&& ($arrayNavigationItems[$nIndex]->m_nLevel >= $nStartLevel)
					&& ($arrayNavigationItems[$nIndex]->m_nLevel <= $nEndLevel)
					&& ($arrayNavigationItems[$nIndex]->m_bConditionCompliance == true))
				{
					$strOutput .= $this->RenderNavigationItem(
						$arrayNavigationItems[$nIndex],
						$navtree->GetContextParameterName(),
						$nStartLevel) . "\n";
					if ($nRenderedEntries < $nVisibleEntries-1)
					{
						$strOutput .= $this->GetLayoutDef("LAYOUT_NAVIGATION_SEPARATOR") . "\n";
					}
					$nRenderedEntries++;
				}
			}
			$strOutput .= $this->GetLayoutDef("LAYOUT_NAVIGATION_END");
			return $strOutput;
		}
		
		
		function RenderNavigationCurrentPathPortfolio(
			CNavigationTree $navtree,
			$strLinkText = "?TID_LINK_NEXT?")
		{
			if ($navtree->IsInitCurrentPath() == false)
			{
				$this->GetWebsite()->Error("IsInitCurrentPath() returned false ");
			}

			
			$arrayNavigationEntries = $navtree->GetNavigationItemArray();
			$nDeepestLevel = $navtree->m_nDeepestLevelVisibleInPathMode;
			
			if ($nDeepestLevel == $navtree->m_nCurrentContextLevel)
			{
				return "";	
			}
			
			$nVisibleEntries = 0;
			for ($nIndex = 0; $nIndex < ArrayCount($arrayNavigationEntries); $nIndex++)
			{
				if (	($arrayNavigationEntries[$nIndex]->m_bVisibleInPathMode == true)
					&&	($arrayNavigationEntries[$nIndex]->m_bConditionCompliance == true)
					&&	($arrayNavigationEntries[$nIndex]->m_nLevel == $nDeepestLevel)
					)
				{
					$nVisibleEntries++;
				}
			}
			
			if ($nVisibleEntries == 0)
			{
				return "";	
			}

			$strPortfolioStyle = $this->GetLayoutDef("LAYOUT_NAVIGATION_PORTFOLIO_STYLE");
			if ($strPortfolioStyle == "")
			{
				$strPortfolioStyle = "default";
			}
			$this->IncludeElement("portfolio");
			$portfolio = new CPortfolioElement($this->GetWebsite(),$strPortfolioStyle);
			$arrayPortfolio = array();

			$nRenderedEntries = 0;
			$strOutput = "";
			for ($nIndex = 0; $nIndex < ArrayCount($arrayNavigationEntries); $nIndex++)
			{
				if (	($arrayNavigationEntries[$nIndex]->m_bVisibleInPathMode == true)
					&&	($arrayNavigationEntries[$nIndex]->m_bConditionCompliance == true)
					&&	($arrayNavigationEntries[$nIndex]->m_nLevel == $nDeepestLevel)
					)
				{
					$arrayTeaser = array();
					$arrayTeaser["title"] = $arrayNavigationEntries[$nIndex]->m_strText;
					$strLink = $arrayNavigationEntries[$nIndex]->m_strLink;
					$strText = $this->GetWebsite()->LoadString("TID_TEXT_".StringUpperCase($strLink));
					$arrayTeaser["text"] = $strText;
					$arrayTeaser["linkurl"] = $arrayNavigationEntries[$nIndex]->GetURL($this->GetWebsite(),$navtree->GetContextParameterName());
					$arrayTeaser["linktext"] = $strLinkText;
					$arrayPortfolio[] = $arrayTeaser;
				}
			}
			$strOutput .= $portfolio->RenderPortfolio($arrayPortfolio);
			return $strOutput;
		}			
		
		
		
	}


