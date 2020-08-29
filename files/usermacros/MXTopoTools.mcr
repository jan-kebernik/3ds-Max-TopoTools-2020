macroScript MXTopoTools
category:"TopoTools"
tooltip:"TopoTools Dialog"
buttontext:"TopoTools"
Icon:#("MXTopoTools", 1)
(
	-- GENERAL NOTES:
	-- =====================================================
	-- Enable Options -> Use Monospaced Font (Ctrl + F11)
	--   Because the default is, for whatever reason, Comic Sans reminiscent. SMH
	-- Long lines/paragraphs will slow down the editor.
	--   Keep each line around ~80 characters, when possible.
	--   Long function calls may not span several lines, though.
	-- Functions always start with prefix "fn" or "fx" and are camelCase.
	-- Arguments always start with prefix "p" and are always camelCase.
	-- Local variables have no prefix and are usually camelCase.
	-- (Private) Global variables have various prefixes ("t_", "s_", etc..) and are always snake_case.
	
	-- TODO
	-- make "Bad key combination..." a commong string
	
	---------------------------------------------------
	---------------------------------------------------
	--                Global callbacks               --
	---------------------------------------------------
	---------------------------------------------------
	-- These callbacks must be global. Sadly, there is just no way around it
	-- There are, ostensibly, other ways to detect the same stuff with non-globals, 
	-- but in practice, it just doesn't work. So, we're stuck with this shit.
	global fnMXTopoToolsCallbackSceneReset
	global fnMXTopoToolsCallbackNodeNameChanged
	
	---------------------------------------------------
	---------------------------------------------------
	--          Published function aliases           --
	---------------------------------------------------
	---------------------------------------------------
	--
	-- All functions will return either some non-negative number which can mean anything based
	-- on context, or they return a negative number that identifies some broad category of error.
	-- Use fm_message() with the error value as an argument to retrieve a string that briefly 
	-- describes the error code in question.
	-- If you ever encounter an actual exception, that is a bug!
	-- You should send the stack-trace of the exception to the author with a general description of what you 
	-- were doing when it occured. Try to give a reproducible case.
	-- Q: Why not rely on exceptions? It's so much more robust and informative!
	-- A: That is true. But exceptions are extremly slow and faulty inputs are very common.
	--    Exceptions should be exceptional. They should be reserved for cases that the programmer could not or did not
	--    account for. And they bring 3ds Max to its knees and make everything screech to a halt with a stop-the-world strategy. 
	--    I'd rather rely on a set of more or less well-defined failure states that leave the program state intact.

	-- misc functions

	local fm_compressIntArray = MXTopoTools.Misc.compressIntArray
	local fm_compressIntArrays = MXTopoTools.Misc.compressIntArrays
	local fm_decompressIntArray = MXTopoTools.Misc.decompressIntArray
	local fm_decompressIntArrays = MXTopoTools.Misc.decompressIntArrays
	local fm_cloneNode = MXTopoTools.Misc.cloneNode
	local fm_buildSwapSequence = MXTopoTools.Misc.buildSwapSequence
	local fm_message = MXTopoTools.Misc.message
	local fm_copyInts = MXTopoTools.Misc.copyInts	-- 0-indexed
	
	-- editable poly related functions
	local fp_isPoly = MXTopoTools.Poly.isPoly
	local fp_getSelection = MXTopoTools.Poly.getSelection
	local fp_setSelection = MXTopoTools.Poly.setSelection
	local fp_setSelection2 = MXTopoTools.Poly.setSelection2
	local fp_deleteFaces = MXTopoTools.Poly.deleteFaces
	local fp_removeEdges = MXTopoTools.Poly.removeEdges
	local fp_weldBorders = MXTopoTools.Poly.weldBorders
	local fp_attach = MXTopoTools.Poly.attach
	local fp_detach = MXTopoTools.Poly.detach
	local fp_unsubdivide = MXTopoTools.Poly.unsubdivide
	local fp_detriangulate = MXTopoTools.Poly.detriangulate
	
	-- skin modifier related funcions
	local fs_skinIndex = MXTopoTools.Skin.skinIndex
	local fs_skinIndices = MXTopoTools.Skin.skinIndices
	local fs_collectUsedBones = MXTopoTools.Skin.collectUsedBones
	local fs_stripWeights = MXTopoTools.Skin.stripWeights
	local fs_stripBones = MXTopoTools.Skin.stripBones
	local fs_getSelection = MXTopoTools.Skin.getSelection
	local fs_setSelection = MXTopoTools.Skin.setSelection
	local fs_setSelection2 = MXTopoTools.Skin.setSelection2
	-- Unsafe functions:
	-- These may be executed in an "undo" context, but may not be undoable. Careful!
	-- Unsafe functions also provide absolutely no guarantees wrt exception handling or memory safety.
	-- However, they generally follow the same conventions as other functions, so get a feel for that.
	-- Basically, they're not that unsafe. Just much more context sensitive than general functions and ususally not undoable.
	-- Seriously though, never call these unless you know exactly what you're doing. Consult the C++ source, if unsure.
	-- These functions in particular are meant to operate on (temporary) clones.
	local fs_addBones = MXTopoTools.Skin.addBones
	local fs_copyWeights = MXTopoTools.Skin.copyWeights
	
	-- ordered functions
	local fo_extractSymmetrySelection = MXTopoTools.Order.extractSymmetrySelection
	local fo_extractTraversalSelection = MXTopoTools.Order.extractTraversalSelection
	local fo_buildSymmetryOrderV = MXTopoTools.Order.buildSymmetryOrderV
	local fo_buildSymmetryOrderE = MXTopoTools.Order.buildSymmetryOrderE
	local fo_buildSymmetryOrderF = MXTopoTools.Order.buildSymmetryOrderF
	local fo_buildTraversalOrder = MXTopoTools.Order.buildTraversalOrder
	local fo_blendMirrorPos = MXTopoTools.Order.blendMirrorPos
	local fo_flipMirrorPos = MXTopoTools.Order.flipMirrorPos
	local fo_blendPos = MXTopoTools.Order.blendPos
	local fo_blendMirrorWeights = MXTopoTools.Order.blendMirrorWeights
	local fo_flipMirrorWeights = MXTopoTools.Order.flipMirrorWeights
	local fo_transferSelection = MXTopoTools.Order.translateSelection
	local fo_transferHandles = MXTopoTools.Order.translateHandles
	local fo_mirrorSelection = MXTopoTools.Order.mirrorSelection
	local fo_mirrorHandles = MXTopoTools.Order.mirrorHandles
	local fo_transferWeights = MXTopoTools.Order.transferWeights
	
	-- GC-able event handler
	local ev_main = undefined
	
	-- UI forward references
	local ROFMain	-- the rolloutFloater
	local UIMain	-- the main rollout, all ui items are accessed via this alias
	
	-- misc forward references
	local fnSetTooltips
	local fnNodePicked
	local fnRemoveNodeData
	local fnRemoveNodeEntries
	local fnFindCADef
	
	-- message log functions/vars
	local fnMsg			-- call this to log a message
	local fnMsgInit		-- called when the tool initialiazes. don't call this directly
	local fnMsgCleanup	-- called when the tool closes. don't call this directly
	local msg_num = #()	-- array containing the message log numbering labels
	local msg_msg = #()	-- array containing the message log message labels
	local msg_cnt = 0	-- the number of messages logged
	local msg_lnc = 0	-- the visible number of lines logged
	
	
	-- misc tool data (survives closing the tool)
	local t_ca = undefined			-- custom attribute
	local t_tips_on = true			-- are tooltips enabled?
	local t_pos_x = 0				-- previous session's x pos
	local t_pos_y = 0				-- previous session's y pos
	local t_has_data = false		-- was tool closed at least once?
	local t_scene_id = undefined	-- the arbitrary id assigned to any single "session" (tool being openened)
	local t_sym_weight = 1.0f
	local t_sym_axis = 1
	local t_sym_affix_a = ""
	local t_sym_affix_b = ""
	local t_sym_affix = 1
	local t_con_weight = 0.0f
	local t_con_check_a = false
	local t_con_check_b = false
	local t_con_out_a = 1.0f
	local t_con_out_b = 1.0f
	local t_con_affix_a = ""
	local t_con_affix_b = ""
	local t_con_affix = 1
	local t_strip_val = 0.0f
	
	local d_handles_valid = false
	local d_handles = #()
	local d_names = #()
	
	-- static node/set data
	local n_idx = 1			-- index of primary node
	local s_idx = 1			-- index of secondary node
	local s_set_idx = 1		-- index of secondary set (local to s. node)
	local s_con_idx = 1		-- index of secondary order set (local to s. node)
	local s_con = #(
		#(),	-- vert conform order for secondary node/set
		#(),	-- edge conform order for secondary node/set
		#()		-- face conform order for secondary node/set
	)
	
	-- copy/paste set data
	local cp_valid = false
	local cp_name = ""
	local cp_bits_v = #()
	local cp_bits_e = #()
	local cp_bits_f = #()
	local cp_bits_h = #()
	
	-- re-usable temp data
	local tmp_ints_0 = #()
	local tmp_ints_1 = #()
	local tmp_ints_2 = #()
	local tmp_ints_3 = #()
	local tmp_ints_4 = #()
	local tmp_ints_5 = #()
	local tmp_ints_6 = #()
	local tmp_ints_7 = #()
	local tmp_ints_8 = #()

	local tmp_bits_0 = #()
	local tmp_bits_1 = #()
	local tmp_bits_2 = #()
	local tmp_bits_3 = #()
	local tmp_nodes = #()
	
	
	-- create and return a new empty instance of node data
	-- this could obviously be done using structs. but POD arrays are more efficient.
	-- and I seriously doubt that MXS could make this faster with structs, anyway. this is fine.
	-- TODO however, it may be worth contracting some of this. the sym data especially need not reside 
	-- in 3 arrays, right?
	fn fnNewNodeData pHandle = (
		-- node data
		#(
			-- [1] node's anim handle
			pHandle,
			-- [2] is currently deleted
			false,
			-- [3] integer data
			#(
				1,			-- [1] node's current set index
				1,			-- [2] node's conform order set index
				1			-- [3] node's symmetry order set index
			),
			-- [4] conform order data
			#(
				#(),		-- [1] vert order
				#(),		-- [2] edge order
				#()			-- [3] face order
			),
			-- [5] symmetry order verts
			#(
				#(),		-- [1] M order
				#(),		-- [2] A order
				#()			-- [3] B order
			),
			-- [6] symmetry order edges
			#(
				#(),		-- [1] M order
				#(),		-- [2] A order
				#()			-- [3] B order
			),
			-- [7] symmetry order faces
			#(
				#(),		-- [1] M order
				#(),		-- [2] A order
				#()			-- [3] B order
			),
			-- [8] s.set list
			#(
				-- s.set data
				#(
					#(),		-- [1] stored verts
					#(),		-- [2] stored edges
					#(),		-- [3] stored faces
					#()			-- [4] stored vert-handles
				)
			),
			-- [9] s.set name list
			#("[None]"),
			-- [10] s.set display list
			#("[None]")
		)
	)
	
	-- primary node data list
	local n_dat = #()
	-- primary node name list
	local n_name = #("[None]")
	-- primary node display list
	local n_disp = #("[None]")
	
	-- filter used to pick scene nodes
	fn fnFilterPick pNode = (
		--(fx_is_poly pNode) > 0
		-- well, it has to be some kind of geometric object, at least
		superClassOf pNode == geometryClass
	)
	
	-- sets ui control's enabled state
	-- doing it this way leads to less flickering without cluttering up everything
	fn fnUpdateUI pUI pSstate = (
		if pUI.enabled != pSstate do (
			pUI.enabled = pSstate
		)
	)
	
	-- set 2 ui controls' enabled state
	fn fnUpdateUI2 pUI1 pUI2 pState = (
		fnUpdateUI pUI1 pState
		fnUpdateUI pUI2 pState
	)
	
	-- set 3 ui controls' enabled state
	fn fnUpdateUI3 pUI1 pUI2 pUI3 pState = (
		fnUpdateUI pUI1 pState
		fnUpdateUI pUI2 pState
		fnUpdateUI pUI3 pState
	)
	
	-- moves list element from a to b, shifting surrounding elements around appropriately
	fn fnMoveItem pList pA pB = (
		local t = pList[pA]
		for x=pA to pB+1 by -1 do (
			pList[x] = pList[x-1]
		)
		for x=pA to pB do (
			pList[x] = pList[x+1]
		)
		pList[pB] = t
	)
	
	-- Given a pre-computed stable swap-sequence X of length N, this is a stable in-place sorting algorithm
	-- that performs in O(N). 
	-- Computing the swap sequence itself takes O(N*log(N)), using a merge-sort.
	-- While this is slower that just sorting directly, it pays off big time when sorting more than one list 
	-- with the same ordering semantics. Rather than performing in O(M*N*Log(N)) for M lists, this allows for 
	-- O(M*N+(N*Log(N))), which is always superior when M > 1. Which is always true for this implementation.
	-- Plus, locality-of-reference is much better with this solution.
	-- Not to mention: The number of lists to be sorted can be changed without affecting the sort itself or its swap semantics at all.
	-- In particular, this swaps each entry in the list with the correct slot from the swap-sequence stored in tmp_ints_0
	fn fnSwapSort pList = (
		local start = 2
		for s in tmp_ints_0 do (
			if s != start do (
				swap pList[start] pList[s]
			)
			start += 1
		)
	)
	
	-- Returns true if the primary node entry describes an existing scene node.
	fn fnExpectValidNode = (
		local n = getAnimByHandle n_dat[n_idx][1]
		if isValidNode n then (
			n
		) else (
			fnMsg "Node currently not present in scene"
			undefined
		)
	)
	
	-- returns the index of node data with the specified handle or 0 if none
	-- never returns 1, because that is reserved for [None] which has no valid handle
	fn fnIndexOfHandle pHandle = (
		local x = 0
		for i = 2 to n_dat.count while x == 0 do (
			if n_dat[i][1] == pHandle do (
				x = i
			)
		)
		x
	)
	
	-- updates the set/node display string for pIdx
	fn fnUpdateDisp pName pDisp pIdx = (
		free pDisp[pIdx]
		if pIdx <= 10 then (
			pDisp[pIdx] = "0" + ((pIdx-1) as string) + ":" + pName[pIdx]
		) else (
			pDisp[pIdx] = ((pIdx-1) as string) + ":" + pName[pIdx]
		)
	)
	
	-- updates the node display string for pIdx
	fn fnUpdateNodeDisp pIdx = (
		if n_dat[pIdx][2] then (
			free n_disp[pIdx]
			n_disp[pIdx] = "*D:" + n_name[pIdx]
		) else (
			fnUpdateDisp n_name n_disp pIdx
		)
	)
	
	-- GLOBAL CALLBACK
	-- called when scene itself is invalidated (reset, load, etc...)
	fn fnMXTopoToolsCallbackSceneReset = (
		d_handles_valid = false
		-- simply remove all node entries
		fnRemoveNodeEntries 2 n_name.count
		fnMsg "Scene reset. Node data lost..."
	)
	
	-- GLOBAL CALLBACK
	-- called when any node whatsoever is renamed
	fn fnMXTopoToolsCallbackNodeNameChanged = (
		local param = callbacks.notificationParam()
		local h = getHandleByAnim param[3]
		local x = fnIndexOfHandle h
		if x != 0 do (
			n_name[x] = param[2]
			fnUpdateNodeDisp x
			UIMain.ui_a05.items = n_disp
			if x == s_idx do (
				UIMain.ui_c02.text = n_disp[x]
			)
		)
	)
	
	-- update related UIs
	fn fnUpdateSymmetryUI pState = (
		if UIMain.ui_d02.enabled != pState do (
			UIMain.ui_d02.enabled = pState
			UIMain.ui_d04.enabled = pState
			UIMain.ui_d05.enabled = pState
			UIMain.ui_d06.enabled = pState
			UIMain.ui_d07.enabled = pState
			UIMain.ui_d08.enabled = pState
			UIMain.ui_d09.enabled = pState
			UIMain.ui_d10.enabled = pState
			UIMain.ui_d11.enabled = pState
			UIMain.ui_d12.enabled = pState
			UIMain.ui_d13.enabled = pState
			UIMain.ui_d14.enabled = pState
			UIMain.ui_d15.enabled = pState
			UIMain.ui_f01.enabled = pState
			UIMain.ui_f02.enabled = pState
			UIMain.ui_f03.enabled = pState
			UIMain.ui_f04.enabled = pState
			UIMain.ui_f05.enabled = pState
		)
	)
	
	-- update related UIs
	fn fnUpdateSymmetrySkinUI pState = (
		if UIMain.ui_h01.enabled != pState do (
			UIMain.ui_h01.enabled = pState
			UIMain.ui_h02.enabled = pState
			UIMain.ui_h03.enabled = pState
			UIMain.ui_h04.enabled = pState
			UIMain.ui_h05.enabled = pState
			UIMain.ui_h06.enabled = pState
			UIMain.ui_h07.enabled = pState
			UIMain.ui_h08.enabled = pState
			UIMain.ui_h09.enabled = pState
		)
	)
	
	-- update related UIs
	fn fnUpdateConformUI pState = (
		if UIMain.ui_e04.enabled != pState do (
			UIMain.ui_e04.enabled = pState
			UIMain.ui_e05.enabled = pState
			UIMain.ui_e06.enabled = pState
			UIMain.ui_e07.enabled = pState
			UIMain.ui_e08.enabled = pState
			UIMain.ui_e09.enabled = pState
			UIMain.ui_e10.enabled = pState
			UIMain.ui_e11.enabled = pState
			UIMain.ui_e12.enabled = pState
			UIMain.ui_g01.enabled = pState
			UIMain.ui_g02.enabled = pState
			UIMain.ui_g03.enabled = pState
			UIMain.ui_g04.enabled = pState
			UIMain.ui_g05.enabled = pState
			UIMain.ui_g06.enabled = pState
			UIMain.ui_g07.enabled = pState
			UIMain.ui_g08.enabled = pState
			UIMain.ui_g09.enabled = pState
		)
	)
	
	-- update related UIs
	fn fnUpdateConformSkinUI pState = (
		if UIMain.ui_e01.enabled != pState do (
			UIMain.ui_e01.enabled = pState
			UIMain.ui_e02.enabled = pState
			UIMain.ui_e03.enabled = pState
			UIMain.ui_i01.enabled = pState
			UIMain.ui_i02.enabled = pState
			UIMain.ui_i03.enabled = pState
			UIMain.ui_i04.enabled = pState
			UIMain.ui_i05.enabled = pState
		)
	)
	
	fn fnGetPrimaryNode = (
		local n = undefined
		if n_idx > 1 do (
			n = getAnimByHandle n_dat[n_idx][1]
			if not isValidNode n do (
				n = undefined
			)
		)
		n
	)
	
	fn fnGetSecondaryNode = (
		local n = undefined
		if s_idx > 1 do (
			n = getAnimByHandle n_dat[s_idx][1]
			if not isValidNode n do (
				n = undefined
			)
		)
		n
	)
	
	fn fnHasSkin pNode = (
		if pNode == undefined then (
			false
		) else (
			local r = fs_skinIndex pNode 1 true
			if r < 0 then (
				fnMsg (fm_message r)
				false
			) else (
				r != 0
			)
		)
	)
	
	-- TODO call when an order is set or invalidated
	-- also when structure changes (skin modifier added/removed or whatever)
	-- but we need to listen to that first
	fn fnEvalDependentUI = (
		local nodeA = fnGetPrimaryNode()
		local nodeB = fnGetSecondaryNode()
		local validA = nodeA != undefined
		local validB = nodeB != undefined
		local orderA = validA and (n_dat[n_idx][3][2] > 1)
		local orderB = orderA and validB and (s_con_idx > 1)
		local symOrd = validA and (n_dat[n_idx][3][3] > 1)
		local skinA = fnHasSkin nodeA
		local skinB = fnHasSkin nodeB
		local validSet = validA and n_dat[n_idx][3][1] > 1
		fnUpdateUI3 UIMain.ui_b01 UIMain.ui_b02 UIMain.ui_b03 validA
		fnUpdateUI UIMain.ui_b04 skinA
		fnUpdateUI3 UIMain.ui_b05 UIMain.ui_b06 UIMain.ui_b07 validSet
		fnUpdateUI UIMain.ui_b08 (skinA and validSet)
		fnUpdateSymmetryUI symOrd
		fnUpdateSymmetrySkinUI (symOrd and skinA)
		fnUpdateConformUI orderB
		fnUpdateConformSkinUI (orderB and skinA and skinB)
		fnUpdateUI UIMain.ui_j01 (validA and validB and nodeA != nodeB)
		fnUpdateUI3 UIMain.ui_j03 UIMain.ui_k01 UIMain.ui_k02 validA
		fnUpdateUI3 UIMain.ui_l01 UIMain.ui_l02 UIMain.ui_l03 skinA
		fnUpdateUI3 UIMain.ui_m01 UIMain.ui_m02 UIMain.ui_m03 validA
	)
	
	fn fnTryEvalDependentUI pHandles = (
		local b = true
		local h_1 = n_dat[n_idx][1]
		local h_2 = n_dat[s_idx][1]
		for h in pHandles while b do (
			if h == h_1 or h == h_2 do (
				fnEvalDependentUI()
				b = false
			)
		)
	)
	
	fn evOther pEv pHandles = (
		fnTryEvalDependentUI pHandles
	)
	
	fn evNodeStructChanged pEv pHandles = (
		fnTryEvalDependentUI pHandles
	)
	
	
	
	
	
	fn fnSelChanged = (
		local sel = getCurrentSelection()
		fnUpdateUI UIMain.ui_a02 (sel.count == 1 and (fnFilterPick sel[1]))
		fnUpdateUI UIMain.ui_j02 (sel.count > 1)
	)
	
	-- only called when selection is different, apparently
	fn evSelChanged pEv pHandles = (
		fnSelChanged()
	)
	
	fn fnUpdateSecondaryOrderButton = (
		local nodeB = fnGetSecondaryNode()
		fnUpdateUI UIMain.ui_c05 (nodeB != undefined and (s_set_idx > 1 or s_con_idx > 1))
	)
	
	fn fnUpdateOrderButtons = (
		local d = n_dat[n_idx]
		local nodeA = fnGetPrimaryNode()
		fnUpdateUI UIMain.ui_a16 (nodeA != undefined and (d[3][1] > 1 or d[3][2] > 1))
		fnUpdateUI UIMain.ui_d01 (nodeA != undefined and (d[3][1] > 1 or d[3][3] > 1))
		fnUpdateSecondaryOrderButton()
	)
	
	-- called when a node is created or deleted (or when tool is re-opened)
	fn fnNodePresenceChanged pObserved = (
		fnSelChanged()
		local c = false
		for i=2 to n_dat.count do (
			local d = n_dat[i]
			local x = not isValidNode (getAnimByHandle d[1])
			if d[2] != x do (
				d[2] = x
				-- stored state different from scene state
				fnUpdateNodeDisp i
				if i == s_idx do (
					UIMain.ui_c02.text = n_disp[i]
				)
				c = true
			)
		)
		if c do (
			UIMain.ui_a05.items = n_disp
			fnEvalDependentUI()
		)
		fnUpdateOrderButtons()
	)
	
	-- called when a node is created or deleted 
	-- instead of going through the supplied handles, we simply re-check stored
	-- nodes. because the number of supplied handles might be huge and have nothing to do
	-- with the stored handles, this is likely to be much faster in bad cases, while still
	-- being fast enough in the average case.
	fn evNodePresenceChanged pEv pHandles = (
		fnNodePresenceChanged true
	)
	
	fn fnSetEntryChangedLight pIdx pCount = (
		fnUpdateUI UIMain.ui_a14 (pIdx > 2)
		fnUpdateUI UIMain.ui_a15 (pIdx > 1 and pIdx < pCount)
	)
	
	-- called when a different set entry is selected, updating UI
	fn fnSetEntryChanged = (
		local d = n_dat[n_idx]
		local i = d[3][1]
		local n = d[9]
		fnSetEntryChangedLight i n.count
		local nodeA = fnGetPrimaryNode()
		fnUpdateUI2 UIMain.ui_a09 UIMain.ui_a12 (i > 1)
		fnUpdateOrderButtons()
		fnUpdateUI3 UIMain.ui_b05 UIMain.ui_b06 UIMain.ui_b07 (i > 1 and nodeA != undefined)
		fnUpdateUI UIMain.ui_b08 (i > 1 and fnHasSkin nodeA)
		fnUpdateUI UIMain.ui_a13 (n.count > 1)
		if i > 1 then (
			UIMain.ui_a12.text = n[i]
		) else (
			UIMain.ui_a12.text = ""	
		)
	)
	
	fn fnSetEntryPicked pIdx = (
		local ints = n_dat[n_idx][3]
		if ints[1] != pIdx do (
			ints[1] = pIdx
			fnSetEntryChanged()
		)
	)
	
	fn fnUpdateSetDisp01 pUI pDisp pIdx pDest = (
		if pIdx == pDest do (
			pUI.text = pDisp[pIdx]
		)
	)
	
	fn fnSetNameChanged pTxt = (
		local d = n_dat[n_idx]
		local ints = d[3]
		local i = ints[1]
		local sname = d[9]
		local sdisp = d[10]
		sname[i] = pTxt
		fnUpdateDisp sname sdisp i
		UIMain.ui_a13.items = sdisp
		fnUpdateSetDisp01 UIMain.ui_a17 sdisp i ints[2]
		fnUpdateSetDisp01 UIMain.ui_d03 sdisp i ints[3]
		if n_idx == s_idx do (
			fnUpdateSetDisp01 UIMain.ui_c04 sdisp i s_set_idx
			fnUpdateSetDisp01 UIMain.ui_c06 sdisp i s_con_idx
		)
	)
	
	fn fnUpdateSetDisp02 pUI pDisp pIdx &pDest = (
		if pIdx <= pDest do (
			pDest += 1
			pUI.text = pDisp[pDest]
		)
	)
	
	fn fnAddSet pName pData = (
		local d = n_dat[n_idx]
		local ints = d[3]
		local x = ints[1] + 1
		local sname = d[9]
		local sdisp = d[10]
		insertItem pData d[8] x
		insertItem pName sname x
		insertItem "" sdisp x
		for i=x to sdisp.count do (
			fnUpdateDisp sname sdisp i
		)
		fnUpdateUI UIMain.ui_a11 true
		UIMain.ui_a13.items = sdisp
		UIMain.ui_a13.selection = x
		ints[1] = x
		-- account for con/sym order sets
		fnUpdateSetDisp02 UIMain.ui_a17 sdisp x &ints[2]
		fnUpdateSetDisp02 UIMain.ui_d03 sdisp x &ints[3]
		if n_idx == s_idx do (
			fnUpdateSetDisp02 UIMain.ui_c04 sdisp x &s_set_idx
			fnUpdateSetDisp02 UIMain.ui_c06 sdisp x &s_con_idx
		)
		fnSetEntryChanged()
	)
	
	-- called when trying to add a new set to current node entry via UI
	fn fnTryAddNewSet = (
		if keyboard.altPressed or keyboard.shiftPressed then (
			fnMsg "Bad key combination..."
		) else (
			local m = undefined
			local s = #(#(),#(),#(),#())
			if keyboard.controlPressed then (
				local n = fnExpectValidNode()
				if n != undefined do (
					local r = fp_isPoly n
					if r < 0 then (
						fnMsg (fm_message r)
					) else (
						local x = fs_skinIndex n 1 true
						if (x < 0) then (
							fnMsg (fm_message x)
						) else (
							if r == 0 and x == 0 then (
								-- neither poly nor skin
								fnMsg "Not editable poly, no enabled Skin mod."
								m = "Set (empty)"
							) else (
								if r == 0 then (
									fnMsg "Not editable poly."
								) else (
									-- collect poly seletiona
									r = fp_getSelection n 0 s[1] 0
									if r == 0 do (
										r = fp_getSelection n 1 s[2] 0
									)
									if r == 0 do (
										r = fp_getSelection n 2 s[3] 0
									)
								)
								if r == 0 and x != 0 do (
									-- collect skin selection
									r = fs_getSelection n x s[4] 0
								)
								if r != 0 then (
									fnMsg (fm_message r)
								) else (
									m = "Set (new)"
								)
							)
						)
					)
				)
			) else (
				m = "Set (empty)"
			)
			if m == undefined then (
				-- might as well free up allocated crap right now
				for i=1 to s.count do (
					free s[i]
					s[i] = undefined
				)
				free s
			) else (
				-- insert into current set list
				fnAddSet m s
			)
		)
	)
	
	fn fnCopySet = (
		local d = n_dat[n_idx]
		local i = d[3][1]
		local s = d[8][i]
		free cp_bits_v
		cp_bits_v = deepcopy s[1]
		free cp_bits_e
		cp_bits_e = deepcopy s[2]
		free cp_bits_f
		cp_bits_f = deepcopy s[3]
		free cp_bits_h
		cp_bits_h = deepcopy s[4]
		free cp_name
		cp_name = deepcopy d[9][i]
		cp_valid = true
		fnUpdateUI UIMain.ui_a10 true
		fnMsg "Set copied"
	)
	
	fn fnPasteSet = (
		fnAddSet (cp_name + "(copy)") #(deepcopy cp_bits_v, deepcopy cp_bits_e, deepcopy cp_bits_f, deepcopy cp_bits_h)
		fnMsg "Set copy added"
	)
	
	fn fnRemoveSetData pDat pIdx = (
		local a = pDat[8][pIdx]
		for i=1 to a.count do (
			free a[i]
			a[i] = undefined
		)
		free a
		pDat[8][pIdx] = undefined
		pDat[9][pIdx] = undefined
		free pDat[10][pIdx]
		pDat[10][pIdx] = undefined
		deleteItem pDat[8] pIdx
		deleteItem pDat[9] pIdx
		deleteItem pDat[10] pIdx
	)
	
	fn fnUpdateSetDisp03 pUI pDisp pStart pEnd &pDest pUpd = (
		if pEnd >= pDest then (
			if pStart <= pDest do (
				pDest = 1
				if pUpd do (
					fnUpdateUI pUI false
				)
				pUI.text = pDisp[1]
			)
		) else (
			pDest -= pEnd - pStart + 1
			pUI.text = pDisp[pDest]
		)
	)
	
	-- remove the range of set entries from pStart inclusive to pEnd inclusive
	fn fnRemoveSetEntries pDat pStart pEnd = (
		local sname = pDat[9]
		local sdisp = pDat[10]
		local ints = pDat[3]
		for i=pEnd to pStart by -1 do (
			fnRemoveSetData pDat i
		)
		for i=pStart to sdisp.count do (
			fnUpdateDisp sname sdisp i
		)
		UIMain.ui_a13.items = sdisp
		if ints[1] > sdisp.count do (
			ints[1] = sdisp.count
			UIMain.ui_a13.selection = sdisp.count
		)
		fnUpdateSetDisp03 UIMain.ui_a17 sdisp pStart pEnd &ints[2] true
		fnUpdateSetDisp03 UIMain.ui_d03 sdisp pStart pEnd &ints[3] true
		if n_idx == s_idx do (
			fnUpdateSetDisp03 UIMain.ui_c04 sdisp pStart pEnd &s_set_idx false
			fnUpdateSetDisp03 UIMain.ui_c06 sdisp pStart pEnd &s_con_idx true
			fnUpdateSecondaryOrderButton()
		)
		fnSetEntryChanged()
		fnUpdateUI UIMain.ui_a11 (sdisp.count > 1)
		fnEvalDependentUI()
	)
	
	-- called when node entries are to be deleted via UI
	fn fnRemoveSetEntry = (
		local a = keyboard.altPressed
		local s = keyboard.shiftPressed
		local c = keyboard.controlPressed
		if a or s or c then (
			if a and s and c then (
				local d = n_dat[n_idx]
				fnRemoveSetEntries d 2 d[9].count
			) else (
				fnMsg "Bad key combination..."
			)
		) else (
			local d = n_dat[n_idx]
			local i = d[3][1]
			if i > 1 then (
				fnRemoveSetEntries d i i
			) else (
				fnMsg "This one cannot be removed..."
			)
		)
	)
	
	fn fnUpdateSetDisp04 pUI pDisp &pDest = (
		if pDest > 1 do (
			local x = tmp_ints_1[pDest - 1]
			if x != pDest do (
				pDest = x
				pUI.text = pDisp[x]
			)
		)
	)
	
	-- sort node entries
	fn fnSortSetEntries pDir = (
		local d = n_dat[n_idx]
		local sname = d[9]
		local sdisp = d[10]
		local ints = d[3]
		fm_buildSwapSequence sname 2 (sname.count-1) &tmp_ints_0 &tmp_ints_1 pDir
		fnSwapSort d[8]
		fnSwapSort sname
		fnSwapSort sdisp
		local upd = false
		local x = 2
		for y in tmp_ints_1 do (
			if x != y do (
				fnUpdateDisp sname sdisp x
				upd = true
			)
			x += 1
		)
		if upd do (
			UIMain.ui_a13.items = sdisp
		)
		x = tmp_ints_1[ints[1] - 1]
		if x != ints[1] do (
			UIMain.ui_a13.selection = x
			ints[1] = x
			fnSetEntryChangedLight x sdisp.count
		)
		fnUpdateSetDisp04 UIMain.ui_a17 sdisp &ints[2]
		fnUpdateSetDisp04 UIMain.ui_d03 sdisp &ints[3]
		if n_idx == s_idx do (
			fnUpdateSetDisp04 UIMain.ui_c04 sdisp &s_set_idx
			fnUpdateSetDisp04 UIMain.ui_c06 sdisp &s_con_idx
		)
	)
	
	fn fnUpdateSetDisp05 pUI pDisp pA pB &pDest = (
		if pA == pDest then (
			pDest = pB
			pUI.text = pDisp[pB]
		) else (
			if pA < pDest then (
				if pB >= pDest do (
					pDest -= 1
					pUI.text = pDisp[pDest]
				)
			) else (
				if pB <= pDest do (
					pDest += 1
					pUI.text = pDisp[pDest]
				)
			)
		)
	)
	
	-- move current node in list by delta
	fn fnMoveSetEntry pDat pA pDelta = (
		local ints = pDat[3]
		local b = pA + pDelta
		local sname = pDat[9]
		local sdisp = pDat[10]
		fnMoveItem pDat[8] pA b
		fnMoveItem sname pA b
		fnMoveItem sdisp pA b
		if pDelta < 0 then (
			for i=b to pA do (
				fnUpdateDisp sname sdisp i
			)
		) else (
			for i=pA to b do (
				fnUpdateDisp sname sdisp i
			)
		)
		UIMain.ui_a13.items = sdisp
		UIMain.ui_a13.selection = b
		ints[1] = b
		fnSetEntryChangedLight b sdisp.count
		fnUpdateSetDisp05 UIMain.ui_a17 sdisp pA b &ints[2]
		fnUpdateSetDisp05 UIMain.ui_d03 sdisp pA b &ints[3]
		if n_idx == s_idx do (
			fnUpdateSetDisp05 UIMain.ui_c04 sdisp pA b &s_set_idx
			fnUpdateSetDisp05 UIMain.ui_c06 sdisp pA b &s_con_idx
		)
	)
	
	-- called when node is to be moved up/down or when nodes are to be sorted via UI
	fn fnMoveOrSortSetEntries pDir = (
		local a = keyboard.altPressed
		local s = keyboard.shiftPressed
		local c = keyboard.controlPressed
		if a or s or c then (
			if (a and s and c) then (
				-- sort ascending/descending
				fnSortSetEntries pDir
			) else (
				-- move to top/bottom
				if (c and not a and not s) then (
					local d = n_dat[n_idx]
					local i = d[3][1]
					if pDir then (
						fnMoveSetEntry d i (2 - i)
					) else (
						fnMoveSetEntry d i (d[9].count - i)
					)
				) else (
					fnMsg "Bad key combination..."
				)
			)
		) else (
			-- move up/down
			local d = n_dat[n_idx]
			local i = d[3][1]
			if pDir then (
				fnMoveSetEntry d i -1
			) else (
				fnMoveSetEntry d i 1
			)
		)
	)
	
	---------------------------------------------------
	---------------------------------------------------
	--        Add, Remove, Move Node entries         --
	---------------------------------------------------
	---------------------------------------------------
	
	-- called when node position has changed, but same node is still active
	-- or simply as part of the heavy variant
	fn fnNodeEntryChangedLight = (
		fnUpdateUI UIMain.ui_a06 (n_idx > 2)
		fnUpdateUI UIMain.ui_a07 (n_idx > 1 and n_idx < n_name.count)
	)
	
	-- called when a different node entry is selected, updating UI
	fn fnNodeEntryChanged = (
		fnNodeEntryChangedLight()
		fnUpdateUI UIMain.ui_a08 (n_idx > 1)
		fnUpdateUI UIMain.ui_a10 (n_idx > 1 and cp_valid)
		local d = n_dat[n_idx]
		local ints = d[3]
		local i = ints[1]
		local sdisp = d[10]
		UIMain.ui_a13.items = sdisp
		UIMain.ui_a13.selection = i
		fnUpdateUI UIMain.ui_a11 (sdisp.count > 1)
		fnUpdateUI UIMain.ui_a17 (ints[2] > 1)
		UIMain.ui_a17.text = sdisp[ints[2]]
		fnUpdateUI UIMain.ui_d03 (ints[3] > 1)
		UIMain.ui_d03.text = sdisp[ints[3]]
		fnSetEntryChanged()
		fnEvalDependentUI()
	)
	
	-- called when a node entry is selected via UI
	fn fnNodeEntryPicked pIdx = (
		if n_idx != pIdx do (
			n_idx = pIdx
			fnNodeEntryChanged()
		)
	)
	
	-- called when a node is to be added via UI
	-- TODO maybe check if scene contains tool data for node?
	-- TODO also check if is poly, update ui then?
	fn fnTryAddNode pNode = (
		local h = getHandleByAnim pNode
		local x = n_idx
		if n_idx == 1 or n_dat[n_idx][1] != h then (
			x = fnIndexOfHandle h
		)
		if x != n_idx do (
			if x == 0 do (
				x = n_idx + 1
				local d = fnNewNodeData h
				insertItem d n_dat x
				insertItem pNode.name n_name x
				insertItem "" n_disp x
				for i=x to n_disp.count do (
					fnUpdateNodeDisp i
				)
				if x <= s_idx do (
					s_idx += 1
					UIMain.ui_c02.text = n_disp[s_idx]
				)
				UIMain.ui_a05.items = n_disp
				fnUpdateUI UIMain.ui_a05 true
				fnUpdateUI UIMain.ui_a04 true
			)
			n_idx = x
			UIMain.ui_a05.selection = x
			fnNodeEntryChanged()
		)
	)
	
	fn fnRemoveNodeData pIdx = (
		local d = n_dat[pIdx]
		
		d[1] = undefined
		d[2] = undefined
		
		free d[3]
		d[3] = undefined
		
		for i=4 to 7 do (
			local a = d[i]
			for j=1 to a.count do (
				free a[j]
				a[j] = undefined
			)
			free a
			d[i] = undefined
		)
		
		local a = d[8]
		for i=1 to a.count do (
			local b = a[i]
			for j=1 to b.count do (
				free b[j]
				b[j] = undefined
			)
			free b
			a[i] = undefined
		)
		free a
		d[8] = undefined
		
		a = d[9]
		for i=1 to a.count do (
			a[i] = undefined
		)
		free a
		d[9] = undefined
		
		a = d[10]
		for i=1 to a.count do (
			free a[i]
			a[i] = undefined
		)
		free a
		d[10] = undefined
		
		free d
		n_dat[pIdx] = undefined
		
		free n_disp[pIdx]
		n_disp[pIdx] = undefined
		
		deleteItem n_dat pIdx
		deleteItem n_name pIdx
		deleteItem n_disp pIdx
	)
	
	-- remove the range of node entries from pStart inclusive to pEnd inclusive
	fn fnRemoveNodeEntries pStart pEnd = (
		for i=pEnd to pStart by -1 do (
			fnRemoveNodeData i
		)
		for i=pStart to n_disp.count do (
			fnUpdateNodeDisp i
		)
		UIMain.ui_a05.items = n_disp
		if n_idx > n_disp.count do (
			n_idx = n_disp.count
			UIMain.ui_a05.selection = n_idx
		)
		if pEnd >= s_idx then (
			if pStart <= s_idx do (
				-- secondary node removed
				s_idx = 1
				s_set_idx = 1
				s_con_idx = 1
				fnUpdateUI UIMain.ui_c06 false
				UIMain.ui_c02.text = n_disp[1]
				UIMain.ui_c04.text = n_disp[1]
				UIMain.ui_c06.text = n_disp[1]
				fnUpdateUI UIMain.ui_c05 false
			)
		) else (
			-- secondary idx changed
			s_idx -= pEnd - pStart + 1
			UIMain.ui_c02.text = n_disp[s_idx]
		)
		fnNodeEntryChanged()
		fnUpdateUI2 UIMain.ui_a04 UIMain.ui_a05 (n_name.count > 1)
	)
	
	-- called when node entries are to be deleted via UI
	fn fnRemoveNodeEntry = (
		local a = keyboard.altPressed
		local s = keyboard.shiftPressed
		local c = keyboard.controlPressed
		if a or s or c then (
			if a and s and c then (
				fnRemoveNodeEntries 2 n_name.count
			) else (
				fnMsg "Bad key combination..."
			)
		) else (
			if n_idx > 1 then (
				fnRemoveNodeEntries n_idx n_idx
			) else (
				fnMsg "This one cannot be removed..."
			)
		)
	)
	
	-- sort node entries
	fn fnSortNodeEntries pDir = (
		fm_buildSwapSequence n_name 2 (n_name.count-1) &tmp_ints_0 &tmp_ints_1 pDir
		fnSwapSort n_dat
		fnSwapSort n_name
		fnSwapSort n_disp
		local upd = false
		local x = 2
		for y in tmp_ints_1 do (
			if x != y do (
				fnUpdateNodeDisp x
				upd = true
			)
			x += 1
		)
		if upd do (
			UIMain.ui_a05.items = n_disp
		)
		x = tmp_ints_1[n_idx - 1]
		if x != n_idx do (
			UIMain.ui_a05.selection = x
			n_idx = x
			fnNodeEntryChangedLight()
		)
		if s_idx > 1 do (
			x = tmp_ints_1[s_idx - 1]
			if x != s_idx do (
				s_idx = x
				UIMain.ui_c02.text = n_disp[x]
			)
		)
	)
	
	-- move current node in list by delta
	fn fnMoveNodeEntry pDelta = (
		local a = n_idx
		local b = n_idx + pDelta
		fnMoveItem n_dat a b
		fnMoveItem n_name a b
		fnMoveItem n_disp a b
		if pDelta < 0 then (
			for i=b to a do (
				fnUpdateNodeDisp i
			)
		) else (
			for i=a to b do (
				fnUpdateNodeDisp i
			)
		)
		UIMain.ui_a05.items = n_disp
		UIMain.ui_a05.selection = b
		n_idx = b
		fnNodeEntryChangedLight()
		-- update secondary node index and display, if affected
		if a < s_idx then (
			if b >= s_idx do (
				s_idx -= 1
				UIMain.ui_c02.text = n_disp[s_idx]
			)
			
		) else (
			if a > s_idx then (
				if b <= s_idx do (
					s_idx += 1
					UIMain.ui_c02.text = n_disp[s_idx]
				)
			) else (
				s_idx = b
				UIMain.ui_c02.text = n_disp[b]
			)
		)
	)
	
	-- called when node is to be moved up/down or when nodes are to be sorted via UI
	fn fnMoveOrSortNodeEntries pDir = (
		local a = keyboard.altPressed
		local s = keyboard.shiftPressed
		local c = keyboard.controlPressed
		if a or s or c then (
			if (a and s and c) then (
				-- sort ascending/descending
				fnSortNodeEntries pDir
			) else (
				-- move to top/bottom
				if (c and not a and not s) then (
					if pDir then (
						fnMoveNodeEntry (2 - n_idx)
					) else (
						fnMoveNodeEntry (n_name.count - n_idx)
					)
				) else (
					fnMsg "Bad key combination..."
				)
			)
		) else (
			-- move up/down
			if pDir then (
				fnMoveNodeEntry -1
			) else (
				fnMoveNodeEntry 1
			)
		)
	)
	
	fn fnSelectNodeInScene = (
		local n = undefined
		if n_idx > 1 do (
			n = getAnimByHandle n_dat[n_idx][1]
			if not isValidNode n do (
				n = undefined
			)
		)
		local sel = getCurrentSelection()
		if n == undefined then (
			if sel.count != 0 do (
				with undo "Select" on (
					clearSelection()
				)
			)
		) else (
			if sel.count != 1 or sel[1] != n do (
				with undo "Select" on (
					select n
				)
			)
		)
	)
	
	-- ---------------------------------------------------
	-- ---------------------------------------------------
	--               actual functionality 
	-- ---------------------------------------------------
	-- ---------------------------------------------------
	
	fn fnGenerateConOrder pD pSetIdx &pConIdx pUI = (
		if keyboard.controlPressed or keyboard.shiftPressed then (
			fnMsg "Bad key combination..."
			false
		) else (
			local i = 1
			local msg = undefined
			if keyboard.altPressed then (
				i = pConIdx
				if i < 2 then (
					fnMsg "Cannot re-generate from nothing..."
				) else (
					msg = "Traversal order re-generated..."
				)
			) else (
				i = pSetIdx
				if i < 2 then (
					fnMsg "Cannot generate from nothing..."
				) else (
					msg = "Traversal order generated..."
				)
			)
			if i > 1 then (
				local n = getAnimByHandle pD[1]
				local s = pD[8][i]
				local ff = 0
				local ee = 0
				local vv = 0
				local r = r = fo_extractTraversalSelection n &s[3] &s[2] &s[1] &ff &ee &vv
				if r == 0 do (
					r = fo_buildTraversalOrder n &s[3] ff ee vv &tmp_ints_0 0
				)
				if r == 0 do (
					r = fo_buildTraversalOrder n &s[3] ff ee vv &tmp_ints_1 1
				)
				if r == 0 do (
					r = fo_buildTraversalOrder n &s[3] ff ee vv &tmp_ints_2 2
				)
				if r == 0 then (
					-- update data/UI
					pConIdx = i
					fnUpdateUI pUI true
					pUI.text = pD[10][i]
					fnEvalDependentUI()
					fnMsg msg
					true
				) else (
					fnMsg (fm_message r)
					false
				)
			) else (
				false
			)
		)
	)
	
	fn fnGeneratePrimaryConOrder = (
		local d = n_dat[n_idx]
		if fnGenerateConOrder d d[3][1] &d[3][2] UIMain.ui_a17 do (
			-- swap order tabs
			swap tmp_ints_0 d[4][1]
			swap tmp_ints_1 d[4][2]
			swap tmp_ints_2 d[4][3]
		)
	)
	
	fn fnGenerateSecondaryConOrder = (
		local d = n_dat[s_idx]
		if fnGenerateConOrder d s_set_idx &s_con_idx UIMain.ui_c06 do (
			-- swap order tabs
			swap tmp_ints_0 s_con[1]
			swap tmp_ints_1 s_con[2]
			swap tmp_ints_2 s_con[3]
		)
	)
	
	fn fnGeneratePrimarySymOrder = (
		if keyboard.controlPressed or keyboard.shiftPressed then (
			fnMsg "Bad key combination..."
		) else (
			local d = n_dat[n_idx]
			local i = 1
			local msg = undefined
			if keyboard.altPressed then (
				i = d[3][3]
				if i < 2 then (
					fnMsg "Cannot re-generate from nothing..."
				) else (
					msg = "Symmetry order re-generated..."
				)
			) else (
				i = d[3][1]
				if i < 2 then (
					fnMsg "Cannot generate from nothing..."
				) else (
					msg = "Symmetry order generated..."
				)
			)
			if i > 1 do (
				local n = getAnimByHandle d[1]
				local s = d[8][i]
				local fA = 0
				local eA = 0
				local vA = 0
				local fB = 0
				local eB = 0
				local vB = 0
				-- t_sym_axis is merely a hint for picking "left"/"right" and is not critical (easily corrected by flipping the sides)
				local r = fo_extractSymmetrySelection n &s[3] &s[2] &s[1] &fA &eA &vA &fB &eB &vB (t_sym_axis-1)
				if r == 0 do (
					r = fo_buildSymmetryOrderV n &s[3] fA eA vA fB eB vB &tmp_ints_0 &tmp_ints_1 &tmp_ints_2
				)
				if r == 0 do (
					r = fo_buildSymmetryOrderE n &s[3] fA eA vA fB eB vB &tmp_ints_3 &tmp_ints_4 &tmp_ints_5
				)
				if r == 0 do (
					r = fo_buildSymmetryOrderF n &s[3] fA eA vA fB eB vB &tmp_ints_6 &tmp_ints_7 &tmp_ints_8
				)
				if r == 0 then (
					-- swap verts (M/A/B)
					swap tmp_ints_0 d[5][1]
					swap tmp_ints_1 d[5][2]
					swap tmp_ints_2 d[5][3]
					-- swap edges (M/A/B)
					swap tmp_ints_3 d[6][1]
					swap tmp_ints_4 d[6][2]
					swap tmp_ints_5 d[6][3]
					-- swap faces (M/A/B)
					swap tmp_ints_6 d[7][1]
					swap tmp_ints_7 d[7][2]
					swap tmp_ints_8 d[7][3]
					-- update data/UI
					d[3][3] = i
					fnUpdateUI UIMain.ui_d03 true
					UIMain.ui_d03.text = d[10][i]
					fnEvalDependentUI()
					fnMsg msg
				) else (
					fnMsg (fm_message r)
				)
			)
		)
	)
	
	fn fnSwapSymmetrySides = (
		local d = n_dat[n_idx]
		swap d[5][2] d[5][3]
		swap d[6][2] d[6][3]
		swap d[7][2] d[7][3]
		fnMsg "Symmetry order sides flipped..."
	)
	
	fn fnAssignSecondary = (
		local d = n_dat[n_idx]
		s_set_idx = d[3][1]
		UIMain.ui_c02.text = n_disp[n_idx]
		UIMain.ui_c04.text = d[10][s_set_idx]
		if s_idx != n_idx do (
			-- if node is different, discard secondary conform order, if any
			s_idx = n_idx
			s_con_idx = 1
			fnUpdateUI UIMain.ui_c06 false
			UIMain.ui_c06.text = d[10][1]
		)
		fnUpdateSecondaryOrderButton()
		fnEvalDependentUI()
	)
	
	fn fnLoadSelection pLevel = (
		local a = keyboard.altPressed
		local s = keyboard.shiftPressed
		local c = keyboard.controlPressed
		if (c and s) or (a and pLevel != 0 and pLevel != 3) then (
			fnMsg "Bad key combination..."
		) else (
			local m = 0
			if c or s do (
				if c then (
					m = 1
				) else (
					m = -1
				)
			)
			local d = n_dat[n_idx]
			local n = getAnimByHandle d[1]
			local r = 0
			local msg = undefined
			if a then (
				if pLevel == 0 then (
					with undo "Select stored Handles as Verts" on (
						r = fp_setSelection n 0 &d[8][d[3][1]][4] m
						msg = "Stored Handles selected as Verts..."
					)
				) else (
					with undo "Select stored Verts as Handles" on (
						r = fs_skinIndex n 1 true
						if r > 0 do (
							with undo "Select Stored Handles" on (
								r = fs_setSelection n r &d[8][d[3][1]][1] m
							)
							msg = "Stored Verts selected as Handles..."
						)
					)
				)
			) else (
				if pLevel < 2 then (
					if pLevel == 0 then (
						with undo "Select stored Verts" on (
							r = fp_setSelection n pLevel &d[8][d[3][1]][1] m
						)
						msg = "Stored Verts selected..."
					) else (
						with undo "Select stored Edges" on (
							r = fp_setSelection n pLevel &d[8][d[3][1]][2] m
						)
						msg = "Stored Edges selected..."
					)
				) else (
					if pLevel == 2 then (
						with undo "Select stored Faces" on (
							r = fp_setSelection n pLevel &d[8][d[3][1]][3] m
						)
						msg = "Stored Faces selected..."
					) else (
						r = fs_skinIndex n 1 true
						if r > 0 do (
							with undo "Select stored Handles" on (
								r = fs_setSelection n r &d[8][d[3][1]][4] m
							)
							msg = "Stored Handles selected..."
						)
					)
				)
			)
			if r < 0 then (
				fnMsg (fm_message r)
			) else (
				fnMsg msg
			)
		)
	)
	
	fn fnStoreSelection pLevel = (
		local a = keyboard.altPressed
		local s = keyboard.shiftPressed
		local c = keyboard.controlPressed
		if (c and s) or (a and pLevel != 0 and pLevel != 3) then (
			fnMsg "Bad key combination..."
		) else (
			local m = 0
			if c or s do (
				if c then (
					m = 1
				) else (
					m = -1
				)
			)
			local d = n_dat[n_idx]
			local n = getAnimByHandle d[1]
			local r = 0
			local msg = undefined
			if a then (
				if pLevel == 3 then (
					r = fs_skinIndex n 1 true
					if r > 0 do (
						r = fs_getSelection n r &d[8][d[3][1]][1] m
						msg = "Selected Handles stored as Verts..."
					)
				) else (
					r = fp_getSelection n 0 &d[8][d[3][1]][4] m
					msg = "Selected Verts stored as Handles..."
				)
			) else (
				if pLevel < 2 then (
					if pLevel == 0 then (
						r = fp_getSelection n 0 &d[8][d[3][1]][1] m
						msg = "Selected Verts stored..."
					) else (
						r = fp_getSelection n 1 &d[8][d[3][1]][2] m
						msg = "Selected Edges stored..."
					)
				) else (
					if pLevel == 2 then (
						r = fp_getSelection n 2 &d[8][d[3][1]][3] m
						msg = "Selected Faces stored..."
					) else (
						r = fs_skinIndex n 1 true
						if r > 0 do (
							r = fs_getSelection n r &d[8][d[3][1]][4] m
							msg = "Selected Handles stored..."
						)
					)
				)
			)
			if r < 0 then (
				fnMsg (fm_message r)
			) else (
				fnMsg msg
			)
		)
	)
	
	fn fnAttachBToA = (
		tmp_nodes.count = 1
		tmp_nodes[1] = getAnimByHandle n_dat[s_idx][1]
		local r = 0
		with undo "Attach secondary to primary node" on (
			r = fp_attach (getAnimByHandle n_dat[n_idx][1]) &tmp_nodes
		)
		if r < 0 do (
			fnMsg (fm_message r)
		)
	)
	
	fn fnAttachAll = (
		local sel = getCurrentSelection()
		tmp_nodes.count = sel.count - 1
		for i=1 to tmp_nodes.count do (
			tmp_nodes[i] = sel[i+1]
		)
		local r = 0
		with undo "Attach selected nodes" on (
			r = fp_attach sel[1] &tmp_nodes
		)
		if r < 0 do (
			fnMsg (fm_message r)
		)
	)
	
	fn fnDetach = (
		if keyboard.shiftPressed then (
			fnMsg "Bad key combination..."
		) else (
			local d = n_dat[n_idx]
			local n = getAnimByHandle d[1]
			local r = 0
			local b = undefined
			if keyboard.altPressed then (
				b = d[8][d[3][1]][3]
			) else (
				r = fp_getSelection n 2 &tmp_bits_0 0
				b = tmp_bits_0
			)
			if r == 0 do (
				with undo "Detach faces" on (
					r = fp_detach n &b (not keyboard.controlPressed)
				)
			)
			if r < 0 do (
				fnMsg (fm_message r)
			)
		)
	)
	
	fn fnRemoveFaces = (
		if keyboard.controlPressed or keyboard.shiftPressed then (
			fnMsg "Bad key combination..."
		) else (
			local d = n_dat[n_idx]
			local n = getAnimByHandle d[1]
			local r = 0
			local b = undefined
			if keyboard.altPressed then (
				b = d[8][d[3][1]][3]
			) else (
				r = fp_getSelection n 2 &tmp_bits_0 0
				b = tmp_bits_0
			)
			if r == 0 do (
				with undo "Remove faces" on (
					r = fp_deleteFaces n &b
				)
			)
			if r < 0 do (
				fnMsg (fm_message r)
			)
		)
	)
	
	fn fnRemoveEdges = (
		if keyboard.shiftPressed then (
			fnMsg "Bad key combination..."
		) else (
			local d = n_dat[n_idx]
			local n = getAnimByHandle d[1]
			local r = 0
			local b = undefined
			if keyboard.altPressed then (
				b = d[8][d[3][1]][2]
			) else (
				r = fp_getSelection n 1 &tmp_bits_0 0
				b = tmp_bits_0
			)
			if r == 0 do (
				with undo "Remove edges" on (
					r = fp_removeEdges n &b keyboard.controlPressed
				)
			)
			if r < 0 do (
				fnMsg (fm_message r)
			)
		)
	)
	
	fn fnWeldBorders = (
		if keyboard.shiftPressed then (
			fnMsg "Bad key combination..."
		) else (
			local d = n_dat[n_idx]
			local n = getAnimByHandle d[1]
			local r = 0
			local a = undefined
			local b = undefined
			if keyboard.altPressed then (
				a = d[8][d[3][1]][1]
				b = d[8][d[3][1]][2]
			) else (
				r = fp_getSelection n 0 &tmp_bits_0 0
				a = tmp_bits_0
				if r == 0 do (
					r = fp_getSelection n 1 &tmp_bits_1 0
					b = tmp_bits_1
				)
			)
			if r == 0 do (
				with undo "Weld border verts" on (
					r = fp_weldBorders n &b &a keyboard.controlPressed
				)
			)
			if r < 0 do (
				fnMsg (fm_message r)
			)
		)
	)
	
	fn fnUnsubdivide = (
		if keyboard.controlPressed or keyboard.shiftPressed then (
			fnMsg "Bad key combination..."
		) else (
			local d = n_dat[n_idx]
			local n = getAnimByHandle d[1]
			local r = 0
			local a = undefined
			local b = undefined
			if keyboard.altPressed then (
				a = d[8][d[3][1]][1]
				b = d[8][d[3][1]][3]
			) else (
				r = fp_getSelection n 0 &tmp_bits_0 0
				a = tmp_bits_0
				if r == 0 do (
					r = fp_getSelection n 2 &tmp_bits_1 0
					b = tmp_bits_1
				)
			)
			if r == 0 do (
				r = fp_unsubdivide n &a &b &tmp_bits_2 &tmp_bits_3
				if r >= 0 do (
					-- at least a partial success
					local r2 = 0
					with undo "Unsubdivide" on (
						r2 = fp_removeEdges n &tmp_bits_2 true
					)
					if r2 < 0 do (
						r = r2
					)
					if r > 0 do (
						fnMsg ("Partial success:\n" + (fm_message (0 - r)))
						r = 0
					)
				)
			)
			if r < 0 do (
				fnMsg (fm_message r)
			)
		)
	)
	
	fn fnDetriangulate = (
		if keyboard.shiftPressed then (
			fnMsg "Bad key combination..."
		) else (
			local d = n_dat[n_idx]
			local n = getAnimByHandle d[1]
			local r = 0
			local a = undefined
			if keyboard.altPressed then (
				a = d[8][d[3][1]][3]
			) else (
				r = fp_getSelection n 2 &tmp_bits_0 0
				a = tmp_bits_0
			)
			if r == 0 do (
				r = fp_detriangulate n &a &tmp_bits_1 (not keyboard.controlPressed)
				if r >= 0 do (
					-- at least a partial success
					local r2 = 0
					with undo "Detriangulate" on (
						r2 = fp_removeEdges n &tmp_bits_1 false
					)
					if r2 < 0 do (
						r = r2
					)
					if r > 0 do (
						fnMsg ("Partial success:\n" + (fm_message (0 - r)))
						r = 0
					)
				)
			)
			if r < 0 do (
				fnMsg (fm_message r)
			)
		)
	)
	
	-- ok, you need to selected the base object in the modify panel, or shit breaks
	fn fnStrip = (
		local c = keyboard.controlPressed
		local s = keyboard.shiftPressed
		if keyboard.altPressed or (c and s) then (
			fnMsg "Bad key combination..."
		) else (
			local d = n_dat[n_idx]
			local n = getAnimByHandle d[1]
			local r = fs_skinIndices n 1 true true &tmp_ints_2
			if r == 0 and tmp_ints_2.count != 0 do (
				if c or s then (
					-- strip bones and maybe weights
					if c then (
						with undo "Strip weights/bones" on (
							for x in tmp_ints_2 while r == 0 do (
								r = fs_stripBones n x true t_strip_val
							)
						)
					) else (
						with undo "Strip bones" on (
							for x in tmp_ints_2 while r == 0 do (
								r = fs_stripBones n x false 0.0f
							)
						)
					)
				) else (
					-- strip weights only
					with undo "Strip weights" on (
						for x in tmp_ints_2 while r == 0 do (
							r = fs_stripWeights n x t_strip_val
						)
					)
				)
			)
			if r < 0 do (
				fnMsg (fm_message r)
			)
		)
	)
	
	fn fnStripClone = (
		local d = n_dat[n_idx]
		local n = getAnimByHandle d[1]
		local r = fs_skinIndices n 1 true true &tmp_ints_2
		if r == 0 do (
			with undo "Stripped clone" on (
				local c = fm_cloneNode n
				if c != undefined then (
					-- replace cloned skins with new skins
					for x in tmp_ints_2 do (
						addModifier c (Skin()) before:x
						deleteModifier c x
					)
					-- copy used bones and weights from original skins to new skins
					for x in tmp_ints_2 while r == 0 do (
						-- collect used bones
						r = fs_collectUsedBones n x &tmp_nodes t_strip_val
						if r == 0 do (
							-- add used bones
							r = fs_addBones c x &tmp_nodes
							if r == 0 do (
								-- copy-match used bone weights
								r = fs_copyWeights n (x-1) c (x-1) t_strip_val
							)
						)
						-- TODO find a way to also copy gizmos, envelope data, transforms, etc.
						-- It's not a priority right now. And gizmos should only be done when no more
						-- bones are added/removed anyway. But still.
					)
					if r != 0 do (
						-- brundlefly must die
						delete c
					)
				) else (
					fnMsg "Cloning failed"
				)
			)
		)
		if r < 0 do (
			fnMsg (fm_message r)
		)
	)
	
	fn fnSymSelect pLevel pSide = (
		local c = keyboard.controlPressed
		local s = keyboard.shiftPressed
		if keyboard.altPressed or (c and s) then (
			fnMsg "Bad key combination..."
		) else (
			local m = 0
			if c or s do (
				if c then (
					m = 1
				) else (
					m = -1
				)
			)
			local d = n_dat[n_idx]
			local n = getAnimByHandle d[1]
			local r = 0
			if pLevel < 3 then (
				with undo "Symmetry select" on (
					r = fp_setSelection2 n pLevel &d[pLevel+5][pSide] m
				)
			) else (
				local x = fs_skinIndex n 1 true
				if (x < 0) then (
					fnMsg (fm_message x)
				) else (
					with undo "Symmetry select" on (
						r = fs_setSelection2 n x &d[5][pSide] m
					)
				)
			)
			if r < 0 do (
				fnMsg (fm_message r)
			)
		)
	)
	
	fn fnMirrorSelection pLevel = (
		local c = keyboard.controlPressed
		local s = keyboard.shiftPressed
		if keyboard.altPressed or (c and s) then (
			fnMsg "Bad key combination..."
		) else (
			local m = 0
			if c or s do (
				if c then (
					m = 1
				) else (
					m = -1
				)
			)
			local d = n_dat[n_idx]
			local n = getAnimByHandle d[1]
			local r = 0
			if pLevel < 3 then (
				local s = d[pLevel+5]
				with undo "Mirror selection" on (
					r = fo_mirrorSelection n &s[1] &s[2] &s[3] pLevel m
				)
			) else (
				local x = fs_skinIndex n 1 true
				if (x < 0) then (
					fnMsg (fm_message x)
				) else (
					local s = d[5]
					with undo "Mirror selection" on (
						r = fo_mirrorHandles n x &s[1] &s[2] &s[3] m
					)
				)
			)
			if r < 0 do (
				fnMsg (fm_message r)
			)
		)
	)
	
	fn fnMirrorFlipOrBlend pFn = (
		if keyboard.controlPressed or keyboard.shiftPressed then (
			fnMsg "Bad key combination..."
		) else (
			local d = n_dat[n_idx]
			local n = getAnimByHandle d[1]
			local a = undefined
			local r = 0
			if keyboard.altPressed then (
				a = d[8][d[3][1]][1]
			) else (
				r = fp_getSelection n 0 &tmp_bits_0 0
				a = tmp_bits_0
			)
			if r == 0 do (
				local s = d[5]
				with undo "Mirror-flip positions" on (
					r = pFn n &s[1] &s[2] &s[3] &a (t_sym_axis-1) t_sym_weight
				)
			)
			if r < 0 do (
				fnMsg (fm_message r)
			)
		)
	)
	
	fn fnMirrorFlipWeights = (
		if keyboard.controlPressed or keyboard.shiftPressed then (
			fnMsg "Bad key combination..."
		) else (
			local d = n_dat[n_idx]
			local n = getAnimByHandle d[1]
			local x = fs_skinIndex n 1 true
			if (x < 0) then (
				fnMsg (fm_message x)
			) else (
				local a = undefined
				local r = 0
				if keyboard.altPressed then (
					a = d[8][d[3][1]][4]
				) else (
					r = fs_getSelection n x &tmp_bits_0 0
					a = tmp_bits_0
				)
				if r == 0 do (
					local m = t_sym_affix == 1
					local s = d[5]
					with undo "Flip weights" on (
						r = fo_flipMirrorWeights n x &s[1] &s[2] &s[3] &a t_sym_affix_a m t_sym_affix_b m
					)
				)
				if r < 0 do (
					fnMsg (fm_message r)
				)
			)
		)
	)
	
	fn fnMirrorBlendWeights = (
		if keyboard.shiftPressed then (
			fnMsg "Bad key combination..."
		) else (
			local d = n_dat[n_idx]
			local n = getAnimByHandle d[1]
			local x = fs_skinIndex n 1 true
			if (x < 0) then (
				fnMsg (fm_message x)
			) else (
				local a = undefined
				local r = 0
				if keyboard.altPressed then (
					a = d[8][d[3][1]][4]
				) else (
					r = fs_getSelection n x &tmp_bits_0 0
					a = tmp_bits_0
				)
				if r == 0 do (
					local m = t_sym_affix == 1
					local s = d[5]
					with undo "Flip weights" on (
						r = fo_blendMirrorWeights n x &s[1] &s[2] &s[3] &a t_sym_affix_a m t_sym_affix_b m keyboard.controlPressed
					)
				)
				if r < 0 do (
					fnMsg (fm_message r)
				)
			)
		)
	)
	
	fn fnConformSelection pDatA pOrdersA pDatB pOrdersB pLevel = (
		local c = keyboard.controlPressed
		local s = keyboard.shiftPressed
		if c and s then (
			fnMsg "Bad key combination..."
		) else (
			local m = 0
			if c or s do (
				if c then (
					m = 1
				) else (
					m = -1
				)
			)
			local n = getAnimByHandle pDatA[1]
			local p = getAnimByHandle pDatB[1]
			local r = 0
			if pLevel < 3 then (
				local a = undefined
				if keyboard.altPressed then (
					a = pDatA[8][pDatA[3][1]][pLevel+1]
				) else (
					r = fp_getSelection n pLevel &tmp_bits_0 0
					a = tmp_bits_0
				)
				--fnMsg ("bla: " + a as string)
				if r == 0 do (
					with undo "Transfer selection" on (
						r = fo_transferSelection n &pOrdersA[pLevel+1] p &pOrdersB[pLevel+1] &a pLevel m
					)
				)
			) else (
				local y = fs_skinIndex p 1 true
				if y < 0 then (
					r = y
				) else (
					local x = fs_skinIndex n 1 true
					if x < 0 then (
						r = x
					) else (
						local a = undefined
						if keyboard.altPressed then (
							a = pDatA[8][pDatA[3][1]][4]
						) else (
							r = fs_getSelection n x &tmp_bits_0 0
							a = tmp_bits_0
						)
						if r == 0 do (
							with undo "Transfer selection" on (
								r = fo_transferHandles n x &pOrdersA[1] p y &pOrdersB[1] &a m
							)
						)
					)
				)
			)
			if r < 0 do (
				fnMsg (fm_message r)
			)
		)
	)
	
	fn fnConformSelectionAtoB pLevel = (
		local d = n_dat[n_idx]
		fnConformSelection d d[4] n_dat[s_idx] s_con pLevel
	)
	
	fn fnConformSelectionBtoA pLevel = (
		local d = n_dat[n_idx]
		fnConformSelection n_dat[s_idx] s_con d d[4] pLevel
	)
	
	fn fnConformBlendPos pUseA pUseB = (
		local d = n_dat[n_idx]
		local n = getAnimByHandle d[1]
		local f = n_dat[s_idx]
		local p = getAnimByHandle f[1]
		local selA = undefined
		local r = 0
		if pUseA and not t_con_check_a then (
			r = fp_getSelection n 0 &tmp_bits_0 0
			selA = tmp_bits_0
		) else (
			selA = d[8][d[3][1]][1]
		)
		if r == 0 do (
			local selB = undefined
			if pUseB and not t_con_check_b then (
				r = fp_getSelection p 0 &tmp_bits_1 0
				selB = tmp_bits_1
			) else (
				selB = f[8][f[3][1]][1]
			)
			if r == 0 do (
				with undo "Conform positions" on (
					r = fo_blendPos n &d[4][1] p &s_con[1] (1.0f-t_con_weight) t_con_out_a t_con_out_b &selA &selB pUseA pUseB
				)
			)
		)
		if r < 0 do (
			fnMsg (fm_message r)
		)
	)
	
	fn fnTransferWeights pDatA pOrdersA pDatB pOrdersB = (
		if keyboard.controlPressed or keyboard.shiftPressed then (
			fnMsg "Bad key combination..."
		) else (
			local r = 0
			local n = getAnimByHandle pDatA[1]
			local x = fs_skinIndex n 1 true
			if x < 0 then (
				r = x
			) else (
				local p = getAnimByHandle pDatB[1]
				local y = fs_skinIndex p 1 true
				if y < 0 then (
					r = y
				) else (
					local a = undefined
					if keyboard.altPressed then (
						a = pDatA[8][pDatA[3][1]][4]
					) else (
						r = fs_getSelection n x &tmp_bits_0 0
						a = tmp_bits_0
					)
					if r == 0 do (
						local m = t_con_affix == 1
						with undo "Transfer weights" on (
							r = fo_transferWeights n x &pOrdersA[1] p y &pOrdersB[1] &a t_con_affix_a m t_con_affix_b m
						)
					)
				)
			)
			if r < 0 do (
				fnMsg (fm_message r)
			)
		)
	)
	
	-- find definition by id without having to first define it (ie. without modifying the scene)
	fn fnIndexOfCADef pDefs = (
		local x = 0
		if pDefs != undefined do (
			for i=1 to pDefs.count while x == 0 do (
				if pDefs[i].attribID[1] == 0x15311759 and pDefs[i].attribID[2] == 0x233776c4 do (
					x = i
				)
			)
		)
		x
	)
	
	fn fnFindCADef = (
		local defs = custAttributes.getDefs rootNode baseObject:false
		local def = undefined
		local x = fnIndexOfCADef defs
		if x != 0 do (
			def = defs[x]
		)
		def
	)
	
	-- custom attribute definition
	-- by storing it all in the scene root, we avoid a bunch of issues
	-- wrt to (un-)deleted nodes and we won't ever have to scan the entire scene either.
	-- Compressed data is packed using a custom differential PFOR implementation.
	-- before i implement this: is this everything? And does it make sense?
	fn fnDefineCA = (
		t_ca = attributes MXTopoToolsCA version:1 attribID:#(0x15311759, 0x233776c4) (
			parameters MXTopoToolsCA_Data (
				-- UI state
				d_tips_on type:#boolean default:true animatable:false
				d_con_check_a type:#boolean default:false animatable:false
				d_con_check_b type:#boolean default:false animatable:false
				d_sym_axis type:#integer default:1 animatable:false
				d_sym_affix type:#integer default:1 animatable:false
				d_con_affix type:#integer default:1 animatable:false
				d_sym_weight type:#float default:1.0f animatable:false
				d_con_weight type:#float default:0.0f animatable:false
				d_con_out_a type:#float default:1.0f animatable:false
				d_con_out_b type:#float default:1.0f animatable:false
				d_strip_val type:#float default:0.0f animatable:false
				d_sym_affix_a type:#string default:""
				d_sym_affix_b type:#string default:""
				d_con_affix_a type:#string default:""
				d_con_affix_b type:#string default:""
				-- primary node index into node entries
				d_p_node_idx type:#integer default:1 animatable:false
				-- secondary node index into node entries
				d_s_node_idx type:#integer default:1 animatable:false
				-- index of secondary set
				d_s_set_idx type:#integer default:1 animatable:false
				-- index of secondary order set
				d_s_con_idx type:#integer default:1 animatable:false
				-- [COMPRESSED] secondary node traversal order verts
				d_s_con_v type:#intTab tabSize:0 tabSizeVariable:on
				-- [COMPRESSED] secondary node traversal order edges
				d_s_con_e type:#intTab tabSize:0 tabSizeVariable:on
				-- [COMPRESSED] secondary node traversal order faces
				d_s_con_f type:#intTab tabSize:0 tabSizeVariable:on
				-- nodes with entries
				d_nodes type:#nodeTab tabSize:0 tabSizeVariable:on
				-- whether a node was valid when storage occurred
				d_valid type:#intTab tabSize:0 tabSizeVariable:on
				-- per node data
				-- ===================================================
				-- [COMPRESSED] how many sets for each node?
				d_sets type:#intTab tabSize:0 tabSizeVariable:on
				-- name of each set
				d_names type:#stringTab tabSize:0 tabSizeVariable:on
				-- [COMPRESSED] node indices
				-- node entry 1: set index
				-- node entry 1: traversal order set index
				-- node entry 1: symmetry order set index
				-- node entry 2: set index
				-- node entry 2: traversal order set index
				-- node entry 2: symmetry order set index
				-- ...
				d_idx type:#intTab tabSize:0 tabSizeVariable:on
				-- [COMPRESSED] traversal order verts/edges/faces for each node
				-- node entry 1: traversal order verts
				-- node entry 1: traversal order edges
				-- node entry 1: traversal order faces
				-- node entry 2: traversal order verts
				-- node entry 2: traversal order edges
				-- node entry 2: traversal order faces
				-- ...
				d_con type:#intTab tabSize:0 tabSizeVariable:on
				-- [COMPRESSED] length of each array above
				d_con_lens type:#intTab tabSize:0 tabSizeVariable:on
				-- [COMPRESSED] symmetry order middle verts/edges/faces for each node
				-- node entry 1: symmetry order middle verts
				-- node entry 1: symmetry order middle edges
				-- node entry 1: symmetry order middle faces
				-- node entry 2: symmetry order middle verts
				-- node entry 2: symmetry order middle edges
				-- node entry 2: symmetry order middle faces
				-- ...
				d_sym_m type:#intTab tabSize:0 tabSizeVariable:on
				-- [COMPRESSED] length of each array above
				d_sym_m_lens type:#intTab tabSize:0 tabSizeVariable:on
				-- [COMPRESSED] symmetry order "left" verts/edges/faces for each node
				-- node entry 1: symmetry order "left" verts
				-- node entry 1: symmetry order "left" edges
				-- node entry 1: symmetry order "left" faces
				-- node entry 2: symmetry order "left" verts
				-- node entry 2: symmetry order "left" edges
				-- node entry 2: symmetry order "left" faces
				-- ...
				d_sym_a type:#intTab tabSize:0 tabSizeVariable:on
				-- [COMPRESSED] length of each array above
				d_sym_a_lens type:#intTab tabSize:0 tabSizeVariable:on
				-- [COMPRESSED] symmetry order "right" verts/edges/faces for each node
				-- node entry 1: symmetry order "right" verts
				-- node entry 1: symmetry order "right" edges
				-- node entry 1: symmetry order "right" faces
				-- node entry 2: symmetry order "right" verts
				-- node entry 2: symmetry order "right" edges
				-- node entry 2: symmetry order "right" faces
				-- ...
				d_sym_b type:#intTab tabSize:0 tabSizeVariable:on
				-- [COMPRESSED] length of each array above
				d_sym_b_lens type:#intTab tabSize:0 tabSizeVariable:on
				-- [COMPRESSED]
				-- selection set 1: vert bits
				-- selection set 1: edge bits
				-- selection set 1: face bits
				-- selection set 1: handle bits
				-- selection set 2: vert bits
				-- selection set 2: edge bits
				-- selection set 2: face bits
				-- selection set 2: handle bits
				-- ...
				d_bits type:#intTab tabSize:0 tabSizeVariable:on
				-- [COMPRESSED] length of each above bitset (as int array)
				d_bits_lens type:#intTab tabSize:0 tabSizeVariable:on
				
			)
		)
	)
	
	
	local d_handles_valid = false
	local d_handles = #()
	
	fn fnTryCompress pR &pSrc &pDst = (
		pDst.count = 0
		if pR >= 0 do (
			pR = fm_compressIntArray &pSrc &pDst
		)
		pR
	)
	
	fn fnTryDecompress pR &pSrc &pDst = (
		if pR >= 0 do (
			pR = fm_decompressIntArray &pSrc 1 &pDst
		)
		pR
	)
	
	fn fnCollectForStorage x1 x2 x3 y1 y2 y3 = (
		tmp_ints_7.count = 0
		tmp_ints_8.count = 0
		for i = 2 to n_dat.count do (
			local d = n_dat[i]
			join tmp_ints_7 d[x1][y1]
			join tmp_ints_7 d[x2][y2]
			join tmp_ints_7 d[x3][y3]
			append tmp_ints_8 d[x1][y1].count
			append tmp_ints_8 d[x2][y2].count
			append tmp_ints_8 d[x3][y3].count
		)
	)
	
	fn fnExtractCollected xDat x1 x2 x3 y1 y2 y3 = (
		local k = 0
		local t = 0
		local m = 0
		for i = 2 to xDat.count do (
			local d = xDat[i]
			
			m = tmp_ints_8[k + 1]
			fm_copyInts &tmp_ints_7 t &d[x1][y1] 0 m 3
			t += m
			
			m = tmp_ints_8[k + 2]
			fm_copyInts &tmp_ints_7 t &d[x2][y2] 0 m 3
			t += m
			
			m = tmp_ints_8[k + 3]
			fm_copyInts &tmp_ints_7 t &d[x2][y3] 0 m 3
			t += m
			
			k += 3
		)
	)
	
	fn fnSceneDataDelete = (
		local defs = custAttributes.getDefs rootNode baseObject:false
		local x = fnIndexOfCADef defs
		if x == 0 then (
			fnMsg "No data stored in scene."
		) else (
			custAttributes.delete rootNode x baseObject:false
			d_handles_valid = false
			fnMsg "Tool data deleted from scene."
		)
	)
	
	fn fnSceneDataLoad = (
		local def = fnFindCADef()
		if def == undefined then (
			fnMsg "No data stored in scene."
		) else (
			local ca = rootNode.MXTopoToolsCA
			
			local xDat = #()
			local xName = #()
			if d_handles_valid then (
				
				xDat.count = d_handles.count + 1
				xDat[1] = fnNewNodeData 0P
				
				xName.count = d_handles.count + 1
				xName[1] = "[None]"
				
				for i = 2 to xDat.count do (
					local h = d_handles[i - 1]
					xDat[i] = fnNewNodeData h
					xDat[i][2] = not isValidNode (getAnimByHandle h)
					xName[i] = d_names[i - 1]
				)
			) else (
				
				local v = #{}
				MXTopoTools.Misc.toBitArray &ca.d_valid &v
				
				xDat.count = ca.d_nodes.count + 1
				xDat[1] = fnNewNodeData 0P
				
				xName.count = ca.d_nodes.count + 1
				xName[1] = "[None]"
				
				for i = 2 to xDat.count do (
					local n = ca.d_nodes[i - 1]
					
					if (not v[i - 1]) or (n == undefined) then (
						xDat[i] = fnNewNodeData (getHandleByAnim rootNode)
						xDat[i][2] = true
						xName[i] = "[???]"	-- this information is not currently available
					) else (
						xDat[i] = fnNewNodeData (getHandleByAnim n)
						if isValidNode n then (
							xDat[i][2] = false
							xName[i] = n.name
						) else (
							xDat[i][2] = true
							xName[i] = "[???]"	-- this information is not currently available
						)
					)
				)
			)
			
			local r = 0
			
			-- extract per node set indices
			r = fnTryDecompress r &ca.d_idx &tmp_ints_8
			if r >= 0 do (
				local k = 0
				for i = 2 to xDat.count do (
					local d = xDat[i][3]
					d[1] = tmp_ints_8[k + 1]
					d[2] = tmp_ints_8[k + 2]
					d[3] = tmp_ints_8[k + 3]
					k += 3
				)
			)
			-- extract per node traveral orders
			r = fnTryDecompress r &ca.d_con &tmp_ints_7
			r = fnTryDecompress r &ca.d_con_lens &tmp_ints_8
			if r >= 0 do (
				fnExtractCollected xDat 4 4 4 1 2 3
			)
			-- extract per node symmetry middle orders
			r = fnTryDecompress r &ca.d_sym_m &tmp_ints_7
			r = fnTryDecompress r &ca.d_sym_m_lens &tmp_ints_8
			if r >= 0 do (
				fnExtractCollected xDat 5 6 7 1 1 1
			)
			-- extract per node symmetry left orders
			r = fnTryDecompress r &ca.d_sym_a &tmp_ints_7
			r = fnTryDecompress r &ca.d_sym_a_lens &tmp_ints_8
			if r >= 0 do (
				fnExtractCollected xDat 5 6 7 2 2 2
			)
			-- extract per node symmetry right orders
			r = fnTryDecompress r &ca.d_sym_b &tmp_ints_7
			r = fnTryDecompress r &ca.d_sym_b_lens &tmp_ints_8
			if r >= 0 do (
				fnExtractCollected xDat 5 6 7 3 3 3
			)
			-- extract number of sets per node
			r = fnTryDecompress r &ca.d_sets &tmp_ints_6
			-- extract set bits for each node
			r = fnTryDecompress r &ca.d_bits &tmp_ints_7
			r = fnTryDecompress r &ca.d_bits_lens &tmp_ints_8
			if r >= 0 do (
				local t = 0
				local m = 0
				local k = 0
				for i = 2 to xDat.count do (
					local numSets = tmp_ints_6[i - 1] + 1
					--fnMsg (numSets as string)
					for j = 2 to numSets do (
						local setBits = #(#(),#(),#(),#())
						xDat[i][8][j] = setBits
						
						m = tmp_ints_8[k + 1]
						fm_copyInts &tmp_ints_7 t &setBits[1] 0 m 3
						t += m
						
						m = tmp_ints_8[k + 2]
						fm_copyInts &tmp_ints_7 t &setBits[2] 0 m 3
						t += m
						
						m = tmp_ints_8[k + 3]
						fm_copyInts &tmp_ints_7 t &setBits[3] 0 m 3
						t += m
						
						m = tmp_ints_8[k + 4]
						fm_copyInts &tmp_ints_7 t &setBits[4] 0 m 3
						t += m
						
						k += 4
					)
				)
			)
			-- extract set names for each node
			if r >= 0 do (
				local k = 1
				for i = 2 to xDat.count do (
					local numSets = tmp_ints_6[i - 1] + 1
					local setNames = xDat[i][9]
					setNames.count = numSets
					for j = 2 to numSets do (
						setNames[j] = ca.d_names[k]
						k += 1
					)
				)
			)
			-- extract secondary node traversal orders
			if r >= 0 do (
				r = fnTryDecompress r &ca.d_s_con_v &tmp_ints_6
				r = fnTryDecompress r &ca.d_s_con_e &tmp_ints_7
				r = fnTryDecompress r &ca.d_s_con_f &tmp_ints_8
			)
			-- finalize
			if r >= 0 then (
				-- replace current with decompressed
				swap s_con[1] tmp_ints_6
				swap s_con[2] tmp_ints_7
				swap s_con[3] tmp_ints_8
				
				n_idx = ca.d_p_node_idx
				s_idx = ca.d_s_node_idx
				s_set_idx = ca.d_s_set_idx
				s_con_idx = ca.d_s_con_idx
				
				for i = n_dat.count to 1 by -1 do (
					fnRemoveNodeData i
				)
				
				
				free n_dat
				n_dat = xDat
				
				free n_name
				n_name = xName
				
				-- rebuild set display list for each node
				for i = 2 to n_dat.count do (
					local d = n_dat[i]
					local sname = d[9]
					local sdisp = d[10]
					sdisp.count = sname.count
					for j = 2 to sname.count do (
						fnUpdateDisp sname sdisp j
					)
				)
				
				
				
				
				n_disp.count = n_name.count
				n_disp[1] = "[None]"
				for i = 2 to n_name.count do (
					fnUpdateNodeDisp i
				)
				UIMain.ui_a05.items = n_disp
				UIMain.ui_a05.selection = n_idx
				
				
				
				fnNodeEntryChanged()
				fnUpdateUI2 UIMain.ui_a04 UIMain.ui_a05 (n_name.count > 1)
				local dsp = n_dat[s_idx][10]
				UIMain.ui_c02.text = n_disp[s_idx]
				UIMain.ui_c04.text = dsp[s_set_idx]
				UIMain.ui_c06.text = dsp[s_con_idx]
				fnUpdateUI UIMain.ui_c06 (s_con_idx > 1)
				fnUpdateSecondaryOrderButton()
				
				fnSelChanged()
				
				-- load UI state
				-- TODO this should be optional (both parts should be)
				if ca.d_tips_on != t_tips_on do (
					fnSetTooltips ca.d_tips_on
				)
				t_con_check_a = ca.d_con_check_a
				t_con_check_b = ca.d_con_check_b
				t_sym_axis = ca.d_sym_axis
				t_sym_affix = ca.d_sym_affix
				t_con_affix = ca.d_con_affix
				t_sym_weight = ca.d_sym_weight
				t_con_weight = ca.d_con_weight
				t_con_out_a = ca.d_con_out_a
				t_con_out_b = ca.d_con_out_b
				t_strip_val = ca.d_strip_val
				t_sym_affix_a = ca.d_sym_affix_a
				t_sym_affix_b = ca.d_sym_affix_b
				t_con_affix_a = ca.d_con_affix_a
				t_con_affix_b = ca.d_con_affix_b
				
				UIMain.ui_g05.checked = t_con_check_a
				UIMain.ui_g07.checked = t_con_check_b
				UIMain.ui_f05.state = t_sym_axis
				UIMain.ui_h09.state = t_sym_affix
				UIMain.ui_i05.state = t_con_affix
				UIMain.ui_f01.value = t_sym_weight
				UIMain.ui_f02.value = t_sym_weight
				UIMain.ui_g01.value = t_con_weight
				UIMain.ui_g02.value = t_con_weight
				UIMain.ui_g08.value = t_con_out_a
				UIMain.ui_g09.value = t_con_out_b
				UIMain.ui_l02.value = t_strip_val
				UIMain.ui_h07.text = t_sym_affix_a
				UIMain.ui_h08.text = t_sym_affix_b
				UIMain.ui_i03.text = t_con_affix_a
				UIMain.ui_i04.text = t_con_affix_b
				
				
				fnMsg "Tool data loaded."
				
			) else (
				-- loading failed for some reason
				fnMsg (fm_message r)
				
			)
			
		)
	)
	
	fn fnSceneDataStore = (
		
		
		local def = fnFindCADef()
		if def == undefined do (
			if t_ca == undefined do (
				fnDefineCA()
			)
			custAttributes.add rootNode t_ca baseObject:false
			def = t_ca
		)
		local ca = rootNode.MXTopoToolsCA
		-- store UI state
		ca.d_tips_on = t_tips_on
		ca.d_con_check_a = t_con_check_a
		ca.d_con_check_b = t_con_check_b
		ca.d_sym_axis = t_sym_axis
		ca.d_sym_affix = t_sym_affix
		ca.d_con_affix = t_con_affix
		ca.d_sym_weight = t_sym_weight
		ca.d_con_weight = t_con_weight
		ca.d_con_out_a = t_con_out_a
		ca.d_con_out_b = t_con_out_b
		ca.d_strip_val = t_strip_val
		ca.d_sym_affix_a = t_sym_affix_a
		ca.d_sym_affix_b = t_sym_affix_b
		ca.d_con_affix_a = t_con_affix_a
		ca.d_con_affix_b = t_con_affix_b
		
		ca.d_p_node_idx = n_idx
		ca.d_s_node_idx = s_idx
		ca.d_s_set_idx = s_set_idx
		ca.d_s_con_idx = s_con_idx
		
		local r = 0
		
		-- encode secondary traversal orders
		if r >= 0 do (
			r = fnTryCompress r &s_con[1] &ca.d_s_con_v
			r = fnTryCompress r &s_con[2] &ca.d_s_con_e
			r = fnTryCompress r &s_con[3] &ca.d_s_con_f
		)
		
		
		-- collect nodes themselves
		if r >= 0 do (
			-- nodes that are valid at the time of saving are stored as they are.
			-- other nodes are stored as undefined.
			-- additionally, the nodes' anim handles are stored locally, 
			-- even if currently invalid.
			-- when you load and the handles are available, you restore nodes
			-- from that. else, any undefined "node" is recovered with the 
			-- animhandle of rootNode, which does not qualify as a node, 
			-- and which I suspect will never be deleted/undeleted.
			ca.d_nodes.count = n_dat.count - 1
			d_handles.count = n_dat.count - 1
			d_names.count = n_dat.count - 1
			
			local v = #{}
			v.count = n_dat.count - 1
			for i = 2 to n_dat.count do (
				local x = i - 1
				local n = getAnimByHandle n_dat[i][1]
				if isValidNode n then (
					ca.d_nodes[x] = n
					v[x] = true
				) else (
					ca.d_nodes[x] = undefined
					v[x] = false
				)
				d_handles[x] = n_dat[i][1]
				d_names[x] = n_name[i]
			)
			MXTopoTools.Misc.toBitset &v &ca.d_valid
			
		)
		-- find total number of relevant sets and encode per node set count
		local numSetsTotal = 0
		if r >= 0 do (
			tmp_ints_8.count = n_dat.count - 1
			for i = 2 to n_dat.count do (
				local num = n_dat[i][9].count - 1
				numSetsTotal += num
				tmp_ints_8[i - 1] = num
			)
			r = fnTryCompress r &tmp_ints_8 &ca.d_sets
		)
		-- collect all relveant set names in one array
		if r >= 0 do (
			ca.d_names.count = numSetsTotal
			local k = 1
			for i = 2 to n_dat.count do (
				local names = n_dat[i][9]
				for j = 2 to names.count do (
					ca.d_names[k] = names[j]
					k += 1
				)
			)
		)
		-- collect and encode per node set indices
		if r >= 0 do (
			tmp_ints_8.count = (n_dat.count - 1) * 3
			local k = 0
			for i = 2 to n_dat.count do (
				local idx = n_dat[i][3]
				tmp_ints_8[k + 1] = idx[1]
				tmp_ints_8[k + 2] = idx[2]
				tmp_ints_8[k + 3] = idx[3]
				k += 3
			)
			r = fnTryCompress r &tmp_ints_8 &ca.d_idx
		)
		-- collect and encode per node traversal orders
		if r >= 0 do (
			fnCollectForStorage 4 4 4 1 2 3
			r = fnTryCompress r &tmp_ints_7 &ca.d_con
			r = fnTryCompress r &tmp_ints_8 &ca.d_con_lens
		)
		-- collect and encode per node symmetry middle orders and counts
		if r >= 0 do (
			fnCollectForStorage 5 6 7 1 1 1
			r = fnTryCompress r &tmp_ints_7 &ca.d_sym_m
			r = fnTryCompress r &tmp_ints_8 &ca.d_sym_m_lens
		)
		-- collect and encode per node symmetry middle orders and counts
		if r >= 0 do (
			fnCollectForStorage 5 6 7 2 2 2
			r = fnTryCompress r &tmp_ints_7 &ca.d_sym_a
			r = fnTryCompress r &tmp_ints_8 &ca.d_sym_a_lens
		)
		-- collect and encode per node symmetry middle orders and counts
		if r >= 0 do (
			fnCollectForStorage 5 6 7 3 3 3
			r = fnTryCompress r &tmp_ints_7 &ca.d_sym_b
			r = fnTryCompress r &tmp_ints_8 &ca.d_sym_b_lens
		)
		-- collect and encode per set bitsets
		if r >= 0 do (
			tmp_ints_7.count = 0
			tmp_ints_8.count = 0
			for i = 2 to n_dat.count do (
				local d = n_dat[i][8]
				for j = 2 to d.count do (
					local b = d[j]
					join tmp_ints_7 b[1]
					join tmp_ints_7 b[2]
					join tmp_ints_7 b[3]
					join tmp_ints_7 b[4]
					append tmp_ints_8 b[1].count
					append tmp_ints_8 b[2].count
					append tmp_ints_8 b[3].count
					append tmp_ints_8 b[4].count
				)
			)
			r = fnTryCompress r &tmp_ints_7 &ca.d_bits
			r = fnTryCompress r &tmp_ints_8 &ca.d_bits_lens
		)
		-- finalize
		if r >= 0 then (	
			d_handles_valid = true
			fnMsg "Tool data stored."
		) else (
			-- failed for some reason.
			fnSceneDataDelete()
			fnMsg (fm_message r)
		)
		
	)
	
	-- ---------------------------------------------------
	-- ---------------------------------------------------
	--                  basic UI stuff                  --
	-- ---------------------------------------------------
	-- ---------------------------------------------------
	
	-- called when the tool is closed by user
	fn fnToolClosed = (
		-- clean up some strings
		fnMsgCleanup()
		-- store some inter-session data
		t_pos_x = ROFMain.pos.x
		t_pos_y = ROFMain.pos.y
		t_has_data = true
		ROFMain = undefined
		-- stop listening
		callbacks.removeScripts id:#MXTopoToolsCallbackID
		ev_main = undefined
		-- collect some garbage real quick
		gc light:true
	)
	
	-- called when the script is executed
	-- either fix bad postions if already open, 
	-- or open and retstore old data if possible and setup listeners
	fn fnToolOpened = (
		local w = 280
		local h = 840
		if (ROFMain == undefined) then (
			-- tool is not open
			if t_has_data then (
				-- restore old position
				local max_x = sysInfo.desktopSize.x - w - 20
				local max_y = sysInfo.desktopSize.y - h - 37
				if t_pos_x > max_x do (
					t_pos_x = max_x
				)
				if t_pos_x < 0 do (
					t_pos_x = 0
				)
				if t_pos_y > max_y do (
					t_pos_y = max_y
				)
				if t_pos_y < 0 do (
					t_pos_y = 0
				)
				ROFMain = newrolloutfloater "TopoTools" w h t_pos_x t_pos_y
			) else (
				-- increase heap to 128 MiB (this should be pretty beefy)
				if heapSize < 134217728 do (
					heapSize = 134217728
				)
				-- create initial node data (must only happen once)
				n_dat[1] = fnNewNodeData 0P
				-- use default position
				-- TODO maybe check if scene contains old position
				ROFMain = newrolloutfloater "TopoTools" w h
			)
			addRollout UIMain ROFMain
			fnMsgInit()
			if not t_tips_on do (
				fnSetTooltips false
			)
			callbacks.removeScripts id:#MXTopoToolsCallbackID
			callbacks.addScript #filePostOpen "fnMXTopoToolsCallbackSceneReset()" id:#MXTopoToolsCallbackID persistent:false
			callbacks.addScript #systemPostNew "fnMXTopoToolsCallbackSceneReset()" id:#MXTopoToolsCallbackID persistent:false
			callbacks.addScript #systemPostReset "fnMXTopoToolsCallbackSceneReset()" id:#MXTopoToolsCallbackID persistent:false
			callbacks.addScript #filePostMerge "fnMXTopoToolsCallbackSceneReset()" id:#MXTopoToolsCallbackID persistent:false
			callbacks.addScript #nodeNameSet "fnMXTopoToolsCallbackNodeNameChanged()" id:#MXTopoToolsCallbackID persistent:false
			ev_main = NodeEventCallback selectionChanged:evSelChanged added:evNodePresenceChanged deleted:evNodePresenceChanged modelStructured:evNodeStructChanged extentionChannelChanged:evOther
			
			-- init ui
			UIMain.ui_a05.items = n_disp
			UIMain.ui_a05.selection = n_idx
			fnNodeEntryChanged()
			fnUpdateUI2 UIMain.ui_a04 UIMain.ui_a05 (n_name.count > 1)
			local dsp = n_dat[s_idx][10]
			UIMain.ui_c02.text = n_disp[s_idx]
			UIMain.ui_c04.text = dsp[s_set_idx]
			UIMain.ui_c06.text = dsp[s_con_idx]
			fnUpdateUI UIMain.ui_c06 (s_con_idx > 1)
			fnUpdateSecondaryOrderButton()
			
			
			
			
			
			fnMsg "TopoTools ready!\nClick \"Help\" if you are lost..."
			
			-- the t_scene_id is mostly meaningless. we just needs something that is globally
			-- accessible and changes whenever a new file is opened or whatever.
			-- so that we can tell if the scene was changed while the tool was closed (not listening).
			-- this seems to work ok. root node does not change, neither does root scene.
			local id = t_scene_id
			t_scene_id = getHandleByAnim trackViewNodes[1]
			if t_has_data do (
				if t_scene_id != id then (
					fnMXTopoToolsCallbackSceneReset()
				) else (
					fnNodePresenceChanged false
				)
			)
		) else (
			-- tool is already open. fix bad bounds.
			if UIMain.open == false do (
				UIMain.open = true
				fnMsg "Reset rollout state"
			)
			local change = false
			if ROFMain.size[2] > h do (
				ROFMain.size = [w, h]
				fnMsg "Reset dialog size"
			)
			local max_x = sysInfo.desktopSize.x - w - 20
			local max_y = sysInfo.desktopSize.y - ROFMain.size[2] - 37
			t_pos_x = ROFMain.pos.x
			t_pos_y = ROFMain.pos.y
			if t_pos_x > max_x do (
				t_pos_x = max_x
				change = true
			)
			if t_pos_x < 0 do (
				t_pos_x = 0
				change = true
			)
			if t_pos_y > max_y do (
				t_pos_y = max_y
				change = true
			)
			if t_pos_y < 0 do (
				t_pos_y = 0
				change = true
			)
			if change do (
				ROFMain.pos = [t_pos_x, t_pos_y]
				fnMsg "Reset dialog position"
			)
		)
		fnSelChanged()
	)
	
	-- tooltip strings
	local tt_0 = #("","","","","","","","","","","","","","","","","")
	local tt_a = #(
		"Pick a scene node",
		"Pick selected scene node",
		"Select node in scene",
		"Remove node from list\nAlt+Shift+Ctrl: Remove all nodes from list",
		"The current node",
		"Move node up\nCtrl: Move node to top\nAlt+Shift+Ctrl: Sort nodes by name in ascending order",
		"Move node down\nCtrl: Move node to bottom\nAlt+Shift+Ctrl: Sort nodes by name in descending order",
		"Add a new selection set\nCtrl: Add a new selection set with node's current selections",
		"Copy selection set",
		"Paste selection set",
		"Remove selection set\nAlt+Shift+Ctrl: Remove all selection sets for current node",
		"Rename current selection set",
		"The current selection set",
		"Move selection set up\nCtrl: Move set to top\nAlt+Shift+Ctrl: Sort sets by name in ascending order",
		"Move selection set down\nCtrl: Move set to bottom\nAlt+Shift+Ctrl: Sort sets by name in descending order",
		"Generate traversal order using current set\nAlt: Re-generate with same set",
		"Set used to generate traversal order"
	)
	local tt_b = #(
		"Set node's face selection to stored faces\nCtrl: Add to selection\nShift: Remove from selection",
		"Set node's edge selection to stored edges\nCtrl: Add to selection\nShift: Remove from selection",
		"Set node's vert selection to stored verts\nCtrl: Add to selection\nShift: Remove from selection\nAlt: Set selection to stored handles\nAlt+Ctrl: Add stored handles to selection\nAlt+Shift: Remove stored handles from selection",
		"Set node's top enabled Skin's vert-handle selection to stored handles\nCtrl: Add to selection\nShift: Remove from selection\nAlt: Set selection to stored verts\nAlt+Ctrl: Add stored verts to selection\nAlt+Shift: Remove stored verts from selection",
		"Set stored faces to node's face selection\nCtrl: Add to storage\nShift: Remove from storage",
		"Set stored edges to node's edge selection\nCtrl: Add to storage\nShift: Remove from storage",
		"Set stored verts to node's vert selection\nCtrl: Add to storage\nShift: Remove from storage\nAlt: Set stored handles to vert selection\nAlt+Ctrl: Add verts to stored handles\nAlt+Shift: Remove verts from stored handles",
		"Set stored handles to nodes's top enabled Skin's selected vert-handles\nCtrl: Add to storage\nShift: Remove from storage\nAlt: Set stored verts to handle selection\nCtr+Alt: Add handles to stored verts\nAlt+Shift: Remove handles from stored verts"
	)
	local tt_c = #(
		"",
		"The secondary node",
		"Assign current set as secondary selection set",
		"The secondary selection set",
		"Generate traversal order using secondary set\nAlt: Re-generate with same set",
		"Set used to generate traversal order"
	)
	local tt_d = #(
		"Generate symmetry order using current set\nAlt: Re-generate with same set",
		"Swap the \"left\" and \"right\" sides\nof the symmetry order",
		"Set used to generate symmetry order",
		"Set node's vert selection to \"left\"-side verts\nCtrl: Add to current selection\nShift: Remove from current selection",
		"Set node's vert selection to \"middle\" verts\nCtrl: Add to current selection\nShift: Remove from current selection",
		"Set node's vert selection to \"right\"-side verts\nCtrl: Add to current selection\nShift: Remove from current selection",
		"Mirror node's vert selection\nCtrl: Add to current selection\nShift: Remove from current selection",
		"Set node's edge selection to \"left\"-side edges\nCtrl: Add to current selection\nShift: Remove from current selection",
		"Set node's edge selection to \"middle\" edges\nCtrl: Add to current selection\nShift: Remove from current selection",
		"Set node's edge selection to \"right\"-side edges\nCtrl: Add to current selection\nShift: Remove from current selection",
		"Mirror node's edge selection\nCtrl: Add to current selection\nShift: Remove from current selection",
		"Set node's face selection to \"left\"-side faces\nCtrl: Add to current selection\nShift: Remove from current selection",
		"Set node's face selection to \"middle\" faces\nCtrl: Add to current selection\nShift: Remove from current selection",
		"Set node's face selection to \"right\"-side faces\nCtrl: Add to current selection\nShift: Remove from current selection",
		"Mirror node's face selection\nCtrl: Add to current selection\nShift: Remove from current selection"
	)
	local tt_e = #(
		"Set sencondary node's handle selection to primary node's corresponding handle selection\nCtrl: Add to selection\nShift: Remove from selection\nAlt: Use primary set's stored handles\nAlt+Ctrl: Add stored handles\nAlt+Shift: Remove stored handles",
		"",
		"Set primary node's handle selection to secondary node's corresponding handle selection\nCtrl: Add to selection\nShift: Remove from selection\nAlt: Use secondary set's stored handles\nAlt+Ctrl: Add stored handles\nAlt+Shift: Remove stored handles",
		"Set sencondary node's vert selection to primary node's corresponding vert selection\nCtrl: Add to selection\nShift: Remove from selection\nAlt: Use primary set's stored verts\nAlt+Ctrl: Add stored verts\nAlt+Shift: Remove stored verts",
		"",
		"Set primary node's vert selection to secondary node's corresponding vert selection\nCtrl: Add to selection\nShift: Remove from selection\nAlt: Use secondary set's stored verts\nAlt+Ctrl: Add stored verts\nAlt+Shift: Remove stored verts",
		"Set sencondary node's edge selection to primary node's corresponding edge selection\nCtrl: Add to selection\nShift: Remove from selection\nAlt: Use primary set's stored edges\nAlt+Ctrl: Add stored edges\nAlt+Shift: Remove stored edges",
		"",
		"Set primary node's edge selection to secondary node's corresponding edge selection\nCtrl: Add to selection\nShift: Remove from selection\nAlt: Use secondary set's stored edges\nAlt+Ctrl: Add stored edges\nAlt+Shift: Remove stored edges",
		"Set sencondary node's face selection to primary node's corresponding face selection\nCtrl: Add to selection\nShift: Remove from selection\nAlt: Use primary set's stored faces\nAlt+Ctrl: Add stored faces\nAlt+Shift: Remove stored faces",
		"",
		"Set primary node's face selection to secondary node's corresponding face selection\nCtrl: Add to selection\nShift: Remove from selection\nAlt: Use secondary set's stored faces\nAlt+Ctrl: Add stored faces\nAlt+Shift: Remove stored faces"
	)
	local tt_f = #(
		"Blend weight:\nDetermines the weight of the new positions vs the current positions",
		"Blend weight:\nDetermines the weight of the new positions vs the current positions",
		"Flips or swaps positions of selected verts along mirror axis\n-If only one side is selected, axis position is inverted\n-If both sides are selected, axis positions are swapped\nAlt: Use stored verts instead of selection",
		"Mirrors or equalized positions of selected verts along mirror axis\n-If only one side is selected, counterpart receives mirrored position\n-If both sides are selected, distance is maintained along mirror axis\nAlt: Use stored verts instead of selection",
		"",
		"",
		""
	)
	local tt_g = #(
		"Conform weight:\nDetermines the influence of primary vs secondary selection on calculated positions\n0.0: secondary selection has no influence\n1.0: primary selection has no influence",
		"Conform weight:\nDetermines the influence of primary vs secondary selection on calculated positions\n0.0: secondary selection has no influence\n1.0: primary selection has no influence",
		"Blend positions between primary and secondary node, using only primary node's vert selection",
		"Blend positions between primary and secondary node, using only secondary node's vert selection",
		"If checked, primary node will use stored verts instead of selection",
		"Blend positions between primary and secondary node, using both their vert selections",
		"If checked, secondary node will use stored verts instead of selection",
		"Output weight A:\nDetermines the weight of the new positions for primary node vs its current positions",
		"Output weight B:\nDetermines the weight of the new positions for secondary node vs its current positions"
	)
	local tt_h = #(
		"Set node's top enabled Skin's vert-handle selection to \"left\"-side handles\nCtrl: Add to current selection\nShift: Remove from current selection",
		"Set node's top enabled Skin's vert-handle selection to \"middle\" handles\nCtrl: Add to current selection\nShift: Remove from current selection",
		"Set node's top enabled Skin's vert-handle selection to \"right\"-side handles\nCtrl: Add to current selection\nShift: Remove from current selection",
		"Mirror node's top enabled Skin's vert-handle selection\nCtrl: Add to current selection\nShift: Remove from current selection",
		"Flips bones lists or swaps weight data completely for selected vert-handles\n-If only one side is selected, bone list is replaced with counterpart's\n-If both sides are selected, swaps weight data completely\nAlt: Use stored handles instead of selection",
		"Mirrors/equalizes weights of selected vert-handles\n-If only one side is selected, counterpart receives mirrored weights\n-If both sides are selected, equalize\nCtrl: Flip receiver\nAlt: Use stored handles instead\nAlt+Ctrl: Use handles and flip receiver",
		"The affix (prefix or suffix) used to identify \"left\"-side bones by name",
		"The affix (prefix or suffix) used to identify \"right\"-side bones by name",
		"",
		""
	)
	local tt_i = #(
		"Transfer weight data of primary node's top enabled Skin's selected vert-handles to corresponding handles in secondary node, using affixes to match existing bones\nAlt: Use stored handles instead of selection",
		"Transfer weight data of secondary node's top enabled Skin's selected vert-handles to corresponding handles in primary node, using affixes to match existing bones\nAlt: Use stored handles instead of selection",
		"The affix (prefix or suffix) used to identify primary bones by name",
		"The affix (prefix or suffix) used to identify secondary bones by name",
		"",
		""
	)
	local tt_j = #(
		"Attach secondary node to a clone of primary node\nWeight data for all Skin mods is preserved losslessly",
		"Combine all selected scene nodes into a new node in arbitrary order\nWeight data for all Skin mods is preserved losslessly",
		"Create a new node containing primary node's selected faces only\nWeight data for all Skin mods is preserved losslessly\nCtrl: Delete faces\nAlt: Use stored faces instead of selection\nAlt+Ctrl: Use and delete stored faces"
	)
	local tt_k = #(
		"Deletes selected faces from primary node\nWeight data for all Skin mods is preserved losslessly\nAlt: Use stored faces instead of selection",
		"Removes selected edges from primary node\nWeight data for all Skin mods is preserved losslessly\nCtrl: Delete dangling verts\nAlt: Use stored edges instead of selection\nAlt+Ctrl: Use stored edges and delete dangling verts"
	)
	local tt_l = #(
		"Creates a clone of primary node that contains only un-strippable weight data\nUnlike \"Strip\" this will result in unused bones being truly removed, however envelope data, etc... is lost",
		"Threshold:\nDetermines the cutoff point for weight stripping\nAny weight with influence less than or equal to this value is stripped",
		"Remove from primary node't top enabled Skin all bones from all weight tables that list an influence at or below the threshold\nCtrl: Also remove unused bones from Skin\nShift: Only remove unused bones from Skin (ignore weights)"
	)
	local tt_m = #(
		"Target weld one run of primary node's selected border verts with a second run\nCtrl: Use other run as target\nAlt: Use stored verts instead\nAlt+Ctrl: Use stored verts and use other run as target",
		"Attempt to un-subdivide primary node's selected faces\nIf face selection is closed, must have one vert selected that would survive un-subdivion\nAlt: Use stored selection instead",
		"Attempt to de-triangulate primary nodes's selected faces\nCtrl: Use strict mode\nAlt: Use stored selection instead\nAlt+Ctrl: Use stored selection and strict mode"
	)
	local tt_o = #(
		"Delete tool-specific data from the current scene",
		"Re-load tool-specific data from the current scene",
		"Store tool-specific data in the current scene"
	)
	local tt_p = #(
		"Open the help file\nAlt+Ctrl+Shift: Toggle tooltips"
	)

	fn fnSetTooltips bEnabled = (
		local arr_ui = #(
			#(UIMain.ui_a01,UIMain.ui_a02,UIMain.ui_a03,UIMain.ui_a04,
				UIMain.ui_a05,UIMain.ui_a06,UIMain.ui_a07,UIMain.ui_a08,
				UIMain.ui_a09,UIMain.ui_a10,UIMain.ui_a11,UIMain.ui_a12,
				UIMain.ui_a13,UIMain.ui_a14,UIMain.ui_a15,UIMain.ui_a16,
				UIMain.ui_a17),
			#(UIMain.ui_b01,UIMain.ui_b02,UIMain.ui_b03,UIMain.ui_b04,
				UIMain.ui_b05,UIMain.ui_b06,UIMain.ui_b07,UIMain.ui_b08),
			#(UIMain.ui_c01,UIMain.ui_c02,UIMain.ui_c03,UIMain.ui_c04,
				UIMain.ui_c05,UIMain.ui_c06),
			#(UIMain.ui_d01,UIMain.ui_d02,UIMain.ui_d03,UIMain.ui_d04,
				UIMain.ui_d05,UIMain.ui_d06,UIMain.ui_d07,UIMain.ui_d08,
				UIMain.ui_d09,UIMain.ui_d10,UIMain.ui_d11,UIMain.ui_d12,
				UIMain.ui_d13,UIMain.ui_d14,UIMain.ui_d15),
			#(UIMain.ui_e01,UIMain.ui_e02,UIMain.ui_e03,UIMain.ui_e04,
				UIMain.ui_e05,UIMain.ui_e06,UIMain.ui_e07,UIMain.ui_e08,
				UIMain.ui_e09,UIMain.ui_e10,UIMain.ui_e11,UIMain.ui_e12),
			#(UIMain.ui_f01,UIMain.ui_f02,UIMain.ui_f03,UIMain.ui_f04),
			#(UIMain.ui_g01,UIMain.ui_g02,UIMain.ui_g03,UIMain.ui_g04,
				UIMain.ui_g05,UIMain.ui_g06,UIMain.ui_g07,UIMain.ui_g08,
				UIMain.ui_g09),
			#(UIMain.ui_h01,UIMain.ui_h02,UIMain.ui_h03,UIMain.ui_h04,
				UIMain.ui_h05,UIMain.ui_h06,UIMain.ui_h07,UIMain.ui_h08),
			#(UIMain.ui_i01,UIMain.ui_i02,UIMain.ui_i03,UIMain.ui_i04),
			#(UIMain.ui_j01,UIMain.ui_j02,UIMain.ui_j03),
			#(UIMain.ui_k01,UIMain.ui_k02),
			#(UIMain.ui_l01,UIMain.ui_l02,UIMain.ui_l03),
			#(UIMain.ui_m01,UIMain.ui_m02,UIMain.ui_m03),
			#(UIMain.ui_o01,UIMain.ui_o02,UIMain.ui_o03)
		)
		local arr_tt = undefined
		if bEnabled then (
			arr_tt = #(tt_a,tt_b,tt_c,tt_d,tt_e,tt_f,tt_g,tt_h,tt_i,tt_j,tt_k,tt_l,tt_m,tt_o)
		) else (
			arr_tt = #(tt_0,tt_0,tt_0,tt_0,tt_0,tt_0,tt_0,tt_0,tt_0,tt_0,tt_0,tt_0,tt_0,tt_0)
		)
		for i = 1 to arr_ui.count do (
			local ui = arr_ui[i]
			local tt = arr_tt[i]
			for j = 1 to ui.count do (
				ui[j].tooltip = tt[j]
			)
		)
		t_tips_on = bEnabled
	)

	rollout UIMain "Main" (
		-- Main UI layout carefually pixel-aligned. Hopefully, none of this will have to change.
		pickbutton ui_a01	"Pk"
			filter:fnFilterPick
			offset:[-6,19] 
			width:27
			tooltip:tt_a[1]
			across:8
		button ui_a02		"Sc"
			offset:[-6,19]
			width:27
			enabled:false
			tooltip:tt_a[2]
		button ui_a03		"Sl"
			offset:[-6,19]
			width:27
			tooltip:tt_a[3]
		button ui_a04		"🗙"
			offset:[-6,19]
			width:27
			enabled:false
			tooltip:tt_a[4]
		button ui_b01		"△"
			offset:[7,19]
			width:27
			enabled:false
			tooltip:tt_b[1]
		button ui_b02		"▬"
			offset:[7,19]
			width:27
			enabled:false
			tooltip:tt_b[2]
		button ui_b03		"●"
			offset:[7,19]
			width:27
			enabled:false
			tooltip:tt_b[3]
		button ui_b04		"○"
			offset:[7,19]
			width:27
			enabled:false
			tooltip:tt_b[4]
		dropdownList ui_a05	items:n_disp
			offset:[-4,0]
			width:104
			height:42	-- num entries before scrollbar appears +2
			enabled:false
			tooltip:tt_a[5]
			across:7
		button ui_a06		"˄"
			offset:[56,0]
			width:12
			height:10
			enabled:false
			tooltip:tt_a[6]
		button ui_a07		"˅"
			offset:[22,11]
			width:12
			height:10
			enabled:false
			tooltip:tt_a[7]
		button ui_b05		"△"
			offset:[23,0]
			width:27
			enabled:false
			tooltip:tt_b[5]
		button ui_b06		"▬"
			offset:[19,0]
			width:27
			enabled:false
			tooltip:tt_b[6]
		button ui_b07		"●"
			offset:[15,0]
			width:27
			enabled:false
			tooltip:tt_b[7]
		button ui_b08		"○"
			offset:[11,0]
			width:27
			enabled:false
			tooltip:tt_b[8]
		button ui_a08		"+"
			offset:[-21,-1]
			width:27
			enabled:false
			tooltip:tt_a[8]
			across:4
		button ui_a09		"⮝"
			offset:[-51,-1]
			width:27
			enabled:false
			tooltip:tt_a[9]
		button ui_a10		"⮟"
			offset:[-81,-1]
			width:27
			enabled:false
			tooltip:tt_a[10]
		button ui_a11		"🗙"
			offset:[-111,-1]
			width:27
			enabled:false
			tooltip:tt_a[11]
		edittext ui_a12		""
			offset:[-8,0]
			width:121
			tooltip:tt_a[12]
			enabled:false
			across:3
		label ui_c01		"N:"
			offset:[18,2]
			width:12
			tooltip:tt_c[1]
		label ui_c02		"[None]"
			offset:[-4,2]
			width:98
			tooltip:tt_c[2]
		dropdownList ui_a13	items:#("[None]")
			offset:[-4,0]
			width:104
			height:42	-- num entries before scrollbar appears +2
			enabled:false
			tooltip:tt_a[13]
			across:5
		button ui_a14		"˄"
			offset:[35,0]
			width:12
			height:10
			enabled:false
			tooltip:tt_a[14]
		button ui_a15		"˅"
			offset:[-13,11]
			width:12
			height:10
			enabled:false
			tooltip:tt_a[15]
		button ui_c03		"🠊"
			offset:[-31,0]
			width:16
			tooltip:tt_c[3]
		label ui_c04		"[None]"
			offset:[-20,4]
			width:98
			tooltip:tt_c[4]
		button ui_a16		"∩"
			offset:[-26,-1]
			width:16
			enabled:false
			tooltip:tt_a[16]
			across:4
		label ui_a17		"[None]"
			offset:[-27,3]
			width:98
			enabled:false
			tooltip:tt_a[17]
		button ui_c05		"∩"
			offset:[-13,-1]
			width:16
			enabled:false
			tooltip:tt_c[5]
		label ui_c06		"[None]"
			offset:[-14,3]
			width:98
			enabled:false
			tooltip:tt_c[6]
		groupBox ui_a00		"Node && Set (A)"
			offset:[-10,-171]
			width:129
			height:172
			across:2 
		groupBox ui_b00		"Set Ops"
			offset:[3,-171]
			width:129
			height:72
		groupBox ui_c00		"Node && Set (B)"
			offset:[123,-100]
			width:129
			height:95
		button ui_d01		"∩"
			offset:[-16,19]
			width:16
			height:10
			enabled:false
			tooltip:tt_d[1]
			across:6
		button ui_d02		"↔"
			offset:[-56,30]
			width:16
			height:10
			enabled:false
			tooltip:tt_d[2]
		label ui_d03		"[None]"
			offset:[-37,23]
			width:98
			enabled:false
			tooltip:tt_d[3]
		button ui_e01		"🠊"
			offset:[13,19]
			width:48
			enabled:false
			tooltip:tt_e[1]
		label ui_e02		"○"
			offset:[11,22]
			width:16
			enabled:false
			tooltip:tt_e[2]
		button ui_e03		"🠈"
			offset:[2,19]
			width:48
			enabled:false
			tooltip:tt_e[3]
		button ui_d04		"🠈"
			offset:[-8,0]
			width:27
			across:7
			enabled:false
			tooltip:tt_d[4]
		button ui_d05		"|"
			offset:[-12,0]
			width:27
			enabled:false
			tooltip:tt_d[5]
		button ui_d06		"🠊"
			offset:[-16,0]
			width:27
			enabled:false
			tooltip:tt_d[6]
		button ui_d07		"↔"
			offset:[-20,0]
			width:27
			enabled:false
			tooltip:tt_d[7]
		button ui_e04		"🠊"
			offset:[0,0]
			width:48
			enabled:false
			tooltip:tt_e[4]
		label ui_e05		"●"
			offset:[4,3]
			width:16
			enabled:false
			tooltip:tt_e[5]
		button ui_e06		"🠈"
			offset:[1,0]
			width:48
			enabled:false
			tooltip:tt_e[6]
		button ui_d08		"🠈"
			offset:[-8,0]
			width:27
			enabled:false
			tooltip:tt_d[08]
			across:7
		button ui_d09		"|"
			offset:[-12,0]
			width:27
			enabled:false
			tooltip:tt_d[09]
		button ui_d10		"🠊"
			offset:[-16,0]
			width:27
			enabled:false
			tooltip:tt_d[10]
		button ui_d11		"↔"
			offset:[-20,0]
			width:27
			enabled:false
			tooltip:tt_d[11]
		button ui_e07		"🠊"
			offset:[0,0]
			width:48
			enabled:false
			tooltip:tt_e[7]
		label ui_e08		"▬"
			offset:[2,3]
			width:16
			enabled:false
			tooltip:tt_e[8]
		button ui_e09		"🠈"
			offset:[1,0]
			width:48
			enabled:false
			tooltip:tt_e[9]
		button ui_d12		"🠈"
			offset:[-8,0]
			width:27
			enabled:false
			tooltip:tt_d[12]
			across:7
		button ui_d13		"|"
			offset:[-12,0]
			width:27
			enabled:false
			tooltip:tt_d[13]
		button ui_d14		"🠊"
			offset:[-16,0]
			width:27
			enabled:false
			tooltip:tt_d[14]
		button ui_d15		"↔"
			offset:[-20,0]
			width:27
			enabled:false
			tooltip:tt_d[15]
		button ui_e10		"🠊"
			offset:[0,0]
			width:48
			enabled:false
			tooltip:tt_e[10]
		label ui_e11		"△"
			offset:[3,4]
			width:16
			enabled:false
			tooltip:tt_e[11]
		button ui_e12		"🠈"
			offset:[1,0]
			width:48
			enabled:false
			tooltip:tt_e[12]
		groupBox ui_d00		"Symmetry Selection"
			offset:[-10,-123]
			width:129
			height:124
			across:2 
		groupBox ui_e00		"Conform Selection"
			offset:[3,-123]
			width:129
			height:124
		slider ui_f01		""
			offset:[-1,19]
			width:63
			range:[0.0f,1.0f,t_sym_weight]
			ticks:0
			enabled:false
			tooltip:tt_f[1]
			across:4
		spinner ui_f02		""
			offset:[-20,21]
			width:58
			range:[0.0f,1.0f,t_sym_weight]
			enabled:false
			tooltip:tt_f[2]
		slider ui_g01		""
			offset:[12,19]
			width:63
			range:[0.0f,1.0f,t_con_weight]
			ticks:0
			enabled:false
			tooltip:tt_g[1]
		spinner ui_g02		""
			offset:[-7,19]
			width:58
			range:[0.0f,1.0f,t_con_weight]
			across:3
			enabled:false
			tooltip:tt_g[2]
		button ui_f03		"Flip"
			offset:[-6,-3]
			width:57
			enabled:false
			tooltip:tt_f[3]
			across:4
		button ui_f04		"Blend"
			offset:[-6,-3]
			width:57
			enabled:false
			tooltip:tt_f[4]
		button ui_g03		"A"
			offset:[7,-3]
			width:57
			enabled:false
			tooltip:tt_g[3]
		button ui_g04		"B"
			offset:[7,-3]
			width:57
			enabled:false
			tooltip:tt_g[4]
		radiobuttons ui_f05	labels:#("X", "Y", "Z")
			columns:3
			offset:[26, 3]
			offsets:#([0,0],[3,0],[6,0])
			default:t_sym_axis
			enabled:false
			tooltip:#(tt_f[5],tt_f[6],tt_f[7])
			across:4
		checkbox ui_g05		""
			offset:[69,3]
			width:12
			checked:t_con_check_a
			enabled:false
			tooltip:tt_g[5]
		button ui_g06		"A + B"
			offset:[37,0]
			width:87
			enabled:false
			tooltip:tt_g[6]
		checkbox ui_g07		""
			offset:[54,3]
			width:12
			checked:t_con_check_b
			enabled:false
			tooltip:tt_g[7]
		groupBox ui_f00		"Symmetry"
			offset:[-10,-97]
			width:129
			height:98
			across:2
		groupBox ui_g00		"Conform"
			offset:[3,-97]
			width:129
			height:124
		button ui_h01		"🠈"
			offset:[-11,-7]
			width:27
			enabled:false
			tooltip:tt_h[1]
			across:6
		button ui_h02		"|"
			offset:[-21,-7]
			width:27
			enabled:false
			tooltip:tt_h[2]
		button ui_h03		"🠊"
			offset:[-31,-7]
			width:27
			enabled:false
			tooltip:tt_h[3]
		button ui_h04		"↔"
			offset:[-41,-7]
			width:27
			enabled:false
			tooltip:tt_h[4]
		spinner ui_g08		""
			offset:[-32,-29]
			width:58
			range:[0.0f,1.0f,t_con_out_a]
			enabled:false
			tooltip:tt_g[8]
			across:3
		spinner ui_g09		""
			offset:[-12,-29]
			width:58
			range:[0.0f,1.0f,t_con_out_b]
			enabled:false
			tooltip:tt_g[9]
			across:3
		button ui_h05		"Flip"
			offset:[-6,0]
			width:57
			enabled:false
			tooltip:tt_h[5]
			across:4
		button ui_h06		"Blend"
			offset:[-6,0]
			width:57
			enabled:false
			tooltip:tt_h[6]
		button ui_i01		"🠊"
			offset:[7,0]
			width:57
			enabled:false
			tooltip:tt_i[1]
		button ui_i02		"🠈"
			offset:[7,0]
			width:57
			enabled:false
			tooltip:tt_i[2]
		edittext ui_h07		""
			offset:[-8,0]
			width:61
			text:t_sym_affix_a
			enabled:false
			tooltip:tt_h[7]
			across:4
		edittext ui_h08		""
			offset:[-8,0]
			width:61
			text:t_sym_affix_b
			enabled:false
			tooltip:tt_h[8]
		edittext ui_i03		""
			offset:[5,0]
			width:61
			text:t_con_affix_a
			enabled:false
			tooltip:tt_i[3]
		edittext ui_i04		""
			offset:[5,0]
			width:61
			text:t_con_affix_b
			enabled:false
			tooltip:tt_i[4]
		radiobuttons ui_h09	labels:#("Prefix","Suffix")
			columns:2
			offset:[-4,0]
			offsets:#([1,0],[5,0])
			default:t_sym_affix
			enabled:false
			tooltip:#(tt_h[9],tt_h[10])
			across:2
		radiobuttons ui_i05	labels:#("Prefix","Suffix")
			columns:2
			offset:[9,0]
			offsets:#([1,0],[5,0])
			default:t_con_affix
			enabled:false
			tooltip:#(tt_i[5],tt_i[6])
		groupBox ui_h00		"Symmetry (Skin)"
			offset:[-10,-113]
			width:129
			height:114
			across:2
		groupBox ui_i00		"Transfer Skin"
			offset:[3,-87]
			width:129
			height:88
		button ui_j01		"B🠊A"
			offset:[-10,19]
			width:37
			enabled:false
			tooltip:tt_j[1]
			across:5
		button ui_j02		"All"
			offset:[-18,19]
			width:37
			enabled:false
			tooltip:tt_j[2]
		button ui_j03		"Split"
			offset:[-26,19]
			width:37
			enabled:false
			tooltip:tt_j[3]
		button ui_k01		"Faces"
			offset:[-11,19]
			width:57
			enabled:false
			tooltip:tt_k[1]
		button ui_k02		"Edges"
			offset:[1,19]
			width:57
			enabled:false
			tooltip:tt_k[2]
		groupBox ui_j00		"Attach/Detach"
			offset:[-10,-45]
			width:129
			height:46
			across:2 
		groupBox ui_k00		"Remove"
			offset:[3,-45]
			width:129
			height:46
		button ui_l01		"🠈"
			offset:[-6,19]
			width:37
			enabled:false
			tooltip:tt_l[1]
			across:6
		spinner ui_l02		""
			offset:[-15,22]
			width:38
			range:[0.0f,1.0f,t_strip_val]
			enabled:false
			tooltip:tt_l[2]
		button ui_l03		"Strip"
			offset:[-5,19]
			width:37
			enabled:false
			tooltip:tt_l[3]
		button ui_m01		"Weld"
			offset:[9,19]
			width:37
			enabled:false
			tooltip:tt_m[1]
		button ui_m02		"U.div"
			offset:[9,19]
			width:37
			enabled:false
			tooltip:tt_m[2]
		button ui_m03		"Detri"
			offset:[7,19]
			width:37
			enabled:false
			tooltip:tt_m[3]
		groupBox ui_l00		"Strip Weights"
			offset:[-10,-45]
			width:129
			height:46
			across:2
		groupBox ui_m00		"Misc."
			offset:[3,-45]
			width:129
			height:46
		label ui_n01		""
			align:#left
			offset:[-3,16]
			across:2
		label ui_n02		""
			align:#left
			width:205
			offset:[-79,16]
		label ui_n03		""
			align:#left
			offset:[-3,-5]
			across:2
		label ui_n04		""
			align:#left
			width:205
			offset:[-79,-5]
		label ui_n05		""
			align:#left
			offset:[-3,-5]
			across:2
		label ui_n06		""
			align:#left
			width:205
			offset:[-79,-5]
		label ui_n07		""
			align:#left
			offset:[-3,-5]
			across:2
		label ui_n08		""
			align:#left
			width:205
			offset:[-79,-5]
		label ui_n09		""
			align:#left
			offset:[-3,-5]
			across:2
		label ui_n10		""
			align:#left
			width:205
			offset:[-79,-5]
		label ui_n11		""
			align:#left
			offset:[-3,-5]
			across:2
		label ui_n12		""
			align:#left
			width:205
			offset:[-79,-5]
		label ui_n13		""
			align:#left
			offset:[-3,-5]
			across:2
		label ui_n14		""
			align:#left
			width:205
			offset:[-79,-5]
		label ui_n15		""
			align:#left
			offset:[-3,-5]
			across:2
		label ui_n16		""
			align:#left
			width:205
			offset:[-79,-5]
		groupBox ui_n00		"Message Log"
			offset:[-10,-125]
			width:262
			height:124
		button ui_o01		"Delete"
			offset:[-4,19]
			width:62
			tooltip:tt_o[1]
			across:4
		button ui_o02		"Load"
			offset:[1,19]
			width:62
			tooltip:tt_o[2]
		button ui_o03		"Store"
			offset:[7,19]
			width:62
			tooltip:tt_o[3]
		button ui_p01		"Help"
			offset:[14,19]
			width:42
			tooltip:tt_p[1]
		groupBox ui_o00		"Scene Data"
			offset:[-10,-45]
			width:204
			height:46
			across:2 
		groupBox ui_p00		"Info"
			offset:[78,-45]
			width:54
			height:46
		on UIMain close do (
			-- defer to dedicated function in main body
			fnToolClosed()
		)
		-- Node & Set (A)
		on ui_a01 picked pNode do (
			fnTryAddNode pNode
		)
		on ui_a02 pressed do (
			fnTryAddNode $
		)
		on ui_a03 pressed do (
			fnSelectNodeInScene()
		)
		on ui_a04 pressed do (
			fnRemoveNodeEntry()
		)
		on ui_a05 selected pIdx do (
			fnNodeEntryPicked pIdx
		)
		on ui_a06 pressed do (
			fnMoveOrSortNodeEntries true
		)
		on ui_a07 pressed do (
			fnMoveOrSortNodeEntries false
		)
		on ui_a08 pressed do (
			fnTryAddNewSet()
		)
		on ui_a09 pressed do (
			fnCopySet()
		)
		on ui_a10 pressed do (
			fnPasteSet()
		)
		on ui_a11 pressed do (
			fnRemoveSetEntry()
		)
		on ui_a12 changed pTxt do (
			fnSetNameChanged pTxt
		)
		on ui_a13 selected pIdx do (
			fnSetEntryPicked pIdx
		)
		on ui_a14 pressed do (
			fnMoveOrSortSetEntries true
		)
		on ui_a15 pressed do (
			fnMoveOrSortSetEntries false
		)
		on ui_a16 pressed do (
			fnGeneratePrimaryConOrder()
		)
		-- Set ops
		on ui_b01 pressed do (
			fnLoadSelection 2
		)
		on ui_b02 pressed do (
			fnLoadSelection 1
		)
		on ui_b03 pressed do (
			fnLoadSelection 0
		)
		on ui_b04 pressed do (
			fnLoadSelection 3
		)
		on ui_b05 pressed do (
			fnStoreSelection 2
		)
		on ui_b06 pressed do (
			fnStoreSelection 1
		)
		on ui_b07 pressed do (
			fnStoreSelection 0
		)
		on ui_b08 pressed do (
			fnStoreSelection 3
		)
		-- Node & Set (B)
		on ui_c03 pressed do (
			fnAssignSecondary()
		)
		on ui_c05 pressed do (
			fnGenerateSecondaryConOrder()
		)
		-- Symmetry selection
		on ui_d01 pressed do (
			fnGeneratePrimarySymOrder()
		)
		on ui_d02 pressed do (
			fnSwapSymmetrySides()
		)
		on ui_d04 pressed do (
			fnSymSelect 0 2
		)
		on ui_d05 pressed do (
			fnSymSelect 0 1
		)
		on ui_d06 pressed do (
			fnSymSelect 0 3
		)
		on ui_d07 pressed do (
			fnMirrorSelection 0
		)
		on ui_d08 pressed do (
			fnSymSelect 1 2
		)
		on ui_d09 pressed do (
			fnSymSelect 1 1
		)
		on ui_d10 pressed do (
			fnSymSelect 1 3
		)
		on ui_d11 pressed do (
			fnMirrorSelection 1
		)
		on ui_d12 pressed do (
			fnSymSelect 2 2
		)
		on ui_d13 pressed do (
			fnSymSelect 2 1
		)
		on ui_d14 pressed do (
			fnSymSelect 2 3
		)
		on ui_d15 pressed do (
			fnMirrorSelection 2
		)
		-- Conform selection
		on ui_e01 pressed do (
			fnConformSelectionAtoB 3
		)
		on ui_e03 pressed do (
			fnConformSelectionBtoA 3
		)
		on ui_e04 pressed do (
			fnConformSelectionAtoB 0
		)
		on ui_e06 pressed do (
			fnConformSelectionBtoA 0
		)
		on ui_e07 pressed do (
			fnConformSelectionAtoB 1
		)
		on ui_e09 pressed do (
			fnConformSelectionBtoA 1
		)
		on ui_e10 pressed do (
			fnConformSelectionAtoB 2
		)
		on ui_e12 pressed do (
			fnConformSelectionBtoA 2
		)
		-- Symmetry
		on ui_f01 changed pVal do (
			t_sym_weight = pVal
			ui_f02.value = pVal
		)
		on ui_f02 changed pVal do (
			t_sym_weight = pVal
			ui_f01.value = pVal
		)
		on ui_f03 pressed do (
			fnMirrorFlipOrBlend fo_flipMirrorPos
		)
		on ui_f04 pressed do (
			fnMirrorFlipOrBlend fo_blendMirrorPos
		)
		on ui_f05 changed pVal do (
			t_sym_axis = pVal
		)
		-- Conform
		on ui_g01 changed pVal do (
			t_con_weight = pVal
			ui_g02.value = pVal
		)
		on ui_g02 changed pVal do (
			t_con_weight = pVal
			ui_g01.value = pVal
		)
		on ui_g03 pressed do (
			fnConformBlendPos true false
		)
		on ui_g04 pressed do (
			fnConformBlendPos false true
		)
		on ui_g05 changed pVal do (
			t_con_check_a = pVal
		)
		on ui_g06 pressed do (
			fnConformBlendPos true true
		)
		on ui_g07 changed pVal do (
			t_con_check_b = pVal
		)
		on ui_g08 changed pVal do (
			t_con_out_a = pVal
		)
		on ui_g09 changed pVal do (
			t_con_out_b = pVal
		)
		-- Symmetry (skin)
		on ui_h01 pressed do (
			fnSymSelect 3 2
		)
		on ui_h02 pressed do (
			fnSymSelect 3 1
		)
		on ui_h03 pressed do (
			fnSymSelect 3 3
		)
		on ui_h04 pressed do (
			fnMirrorSelection 3
		)
		on ui_h05 pressed do (
			fnMirrorFlipWeights()
		)
		on ui_h06 pressed do (
			fnMirrorBlendWeights()
		)
		on ui_h07 changed pTxt do (
			t_sym_affix_a = pTxt
		)
		on ui_h08 changed pTxt do (
			t_sym_affix_b = pTxt
		)
		on ui_h09 changed pVal do (
			t_sym_affix = pVal
		)
		-- Transfer skin
		on ui_i01 pressed do (
			local d = n_dat[n_idx]
			fnTransferWeights d d[4] n_dat[s_idx] s_con
		)
		on ui_i02 pressed do (
			local d = n_dat[n_idx]
			fnTransferWeights n_dat[s_idx] s_con d d[4]
		)
		on ui_i03 changed pTxt do (
			t_con_affix_a = pTxt
		)
		on ui_i04 changed pTxt do (
			t_con_affix_b = pTxt
		)
		on ui_i05 changed pVal do (
			t_con_affix = pVal
		)
		-- Attach/detach
		on ui_j01 pressed do (
			fnAttachBToA()
		)
		on ui_j02 pressed do (
			fnAttachAll()
		)
		on ui_j03 pressed do (
			fnDetach()
		)
		-- Remove
		on ui_k01 pressed do (
			fnRemoveFaces()
		)
		on ui_k02 pressed do (
			fnRemoveEdges()
		)
		-- Strip Weights
		on ui_l01 pressed do (
			fnStripClone()
		)
		on ui_l02 changed pVal do (
			t_strip_val = pVal
		)
		on ui_l03 pressed do (
			fnStrip()
		)
		-- Misc.
		on ui_m01 pressed do (
			fnWeldBorders()
		)
		on ui_m02 pressed do (
			fnUnsubdivide()
		)
		on ui_m03 pressed do (
			fnDetriangulate()
		)
		-- Scene Data
		on ui_o01 pressed do (
			fnSceneDataDelete()
		)
		on ui_o02 pressed do (
			fnSceneDataLoad()
		)
		on ui_o03 pressed do (
			fnSceneDataStore()
		)
		-- Info
		on ui_p01 pressed do (
			if (keyboard.altPressed and keyboard.controlPressed and keyboard.shiftPressed) then (
				if t_tips_on then (
					fnSetTooltips false
					fnMsg "Tooltips disabled"
				) else (
					fnSetTooltips true
					fnMsg "Tooltips enabled"
				)
			) else (
				ShellLaunch ((GetDir #userMacros) + "\\MXTopoTools\\Readme.html") ""
			)
		)
	)
	
	-- initialize message log arrays and counts
	fn fnMsgInit = (
		msg_cnt = 0
		msg_lnc = 0
		msg_num[8] = UIMain.ui_n15
		msg_num[7] = UIMain.ui_n13
		msg_num[6] = UIMain.ui_n11
		msg_num[5] = UIMain.ui_n09
		msg_num[4] = UIMain.ui_n07
		msg_num[3] = UIMain.ui_n05
		msg_num[2] = UIMain.ui_n03
		msg_num[1] = UIMain.ui_n01
		msg_msg[8] = UIMain.ui_n16
		msg_msg[7] = UIMain.ui_n14
		msg_msg[6] = UIMain.ui_n12
		msg_msg[5] = UIMain.ui_n10
		msg_msg[4] = UIMain.ui_n08
		msg_msg[3] = UIMain.ui_n06
		msg_msg[2] = UIMain.ui_n04
		msg_msg[1] = UIMain.ui_n02
	)
	
	-- clean up message log arrays and counts
	fn fnMsgCleanup = (
		for i=1 to msg_msg.count do (
			msg_msg[i] = undefined
		)
		for i=1 to msg_num.count do (
			free msg_num[i]
			msg_num[i] = undefined
		)
		msg_msg.count = 0
		msg_num.count = 0
	)
	
	-- Push a single line to the message log. Don't call this directly!
	-- This is called by fnMsg(). Alway use that instead.
	fn fnMsgPush pMsg pNum = (
		if msg_lnc < msg_msg.count then (
			-- simply increase line count if below max
			msg_lnc += 1
		) else (
			-- else: move every line down by one.
			-- the message may not be freed, because there might be references to it,
			-- but the number is never referenced, so free it.
			free msg_num[1].text
			for i=1 to msg_lnc-1 do (
				msg_num[i].text = msg_num[i+1].text
				msg_msg[i].text = msg_msg[i+1].text
			)
		)
		if pNum == 1 then (
			msg_num[msg_lnc].text = msg_cnt as string
		) else (
			msg_num[msg_lnc].text = ""
		)
		msg_msg[msg_lnc].text = pMsg
	)
	
	-- Push an arbitrary message to the message log. The message may contain any number of line breaks.
	-- You should still deal with the formatting manually to make it look nice.
	-- This way, a small history of messages is maintained and no obtrusive and annoying message boxes
	-- need to be used. The point is to inform the user, not harass them.
	-- Changes are always identifiable because the message number will change even if the same message
	-- is pushed repeatedly.
	-- The UI is aligned to support up to 100k pushed messages during the same session. 
	-- Any more will make the number and message labels overlap somewhat.
	-- The message number strings are freed when they are pushed out of the log, but 
	-- the message strings themselves are not. This is because they might be referenced elsewhere. Freeing
	-- them here could break things.
	-- (Btw, this is as good as it gets visually. I've tried with moving the labels intead, and it just
	-- leads to even worse flickering.)
	fn fnMsg pMsg = (
		msg_cnt += 1
		local n = 1
		local p = 1
		for i=1 to pMsg.count do (
			if pMsg[i] == "\n" do (
				local s = substring pMsg p (i-p)
				p = i+1
				fnMsgPush s n
				n += 1
			)
		)
		if n != 1 do (
			pMsg = substring pMsg p (pMsg.count-p+1)
		)
		fnMsgPush pMsg n
		n = msg_lnc - n
		if n < 0 do (
			n = 0
		)
		for i=1 to n do (
			fnUpdateUI msg_num[i] false
			fnUpdateUI msg_msg[i] false
		)
		for i=n+1 to msg_lnc do (
			fnUpdateUI msg_num[i] true
			fnUpdateUI msg_msg[i] true
		)
	)
	
	on execute do (
		fnToolOpened()
	)
)
