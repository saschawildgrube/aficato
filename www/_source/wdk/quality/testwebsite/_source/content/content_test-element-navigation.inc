<?php

	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{

			// Creating a navigation tree "test3" and store it into the CWebsite
			$navtreeDynamic = new CNavigationTree();
			$newItem = new CNavigationItem();
			$newItem->m_nLevel = 0;
			$newItem->m_strText = "Test 3.1";
			$newItem->m_strLink = "test31";
			$newItem->m_bConditionCompliance = true;  
			$navtreeDynamic->AttachItem($newItem);
			$newItem = new CNavigationItem();
			$newItem->m_nLevel = 0;
			$newItem->m_strText = "Test 3.2";
			$newItem->m_strLink = $this->GetWebsite()->MakeURL(array(
				"content" => $this->GetWebsite()->GetContent(),
				"param" => "test32"
				));
			$newItem->m_bConditionCompliance = true;
			$navtreeDynamic->AttachItem($newItem);
			$this->GetWebsite()->SetDynamicNavigationTree("test3",$navtreeDynamic);
			
			
			// Load the main navigation tree "default"
			$navtree = new CNavigationTree();
			$navtree->LoadNavigationTree(
   			$this->GetWebsite(),
   			"default");

			// Attach the navigation tree "test2" to the main tree
			$navtree_attach = new CNavigationTree();
			$navtree_attach->LoadNavigationTree(
   			$this->GetWebsite(),
   			"test2");
			$navtree->AttachNavigationTree(
   			$navtree_attach,
   			"test2",	// attach after node named "test2"
   			1);   		// attach as a sub node of "test2"
 
 
 			// Render the full navigation tree 
			$navtree->SetCurrentContext("test3");
			$navtree->SetContextParameterName("content");
			$navigation = new CNavigationElement($this->GetWebsite(),"verticalpath");
			$arrayItems["ELEMENT_NAVIGATION_VERTICAL"] = $navigation->RenderNavigationVertical($navtree);

		}
	}
	
