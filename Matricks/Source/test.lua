function update(x)
	local forceX, forceY;
	local gLeft, gRight;
	local animeState;
	
	if x == 0 then 
		forceX == forceY = 0.0
		animeState = "idle"
		gLeft == gRight = false
	end

	elseif x == 1 then
		forceX = -0.01
		forceY = 0.0
		animeState = "moveleft"
		gLeft = true
		gRight = false
	end
	elseif x == 2 then
		forceX = 0.01
		forceY = 0.0
		animeState = "moveright"
		gLeft = false
		gRight = true
	end

	return forceX,forceY,animeState,gLeft,gRight
end
