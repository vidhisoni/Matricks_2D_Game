--[[ Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sairaj Padghe
- End Header --------------------------------------------------------]]--
function update(x)
	local values = {}
	if x == 0 then 
		values = {
			forceX = 0.0,
			forceY = 0.0,
			animeState = "idleleft",
			gLeft = 0,
			gRight = 0
		}
	elseif x == 1 then
		values = {
			forceX = 0.0,
			forceY = 0.0,
			animeState = "idleright",
			gLeft = 0,
			gRight = 0
		}
	elseif x == 2 then
		values = {
			forceX = -11050.01,
			forceY = 0.0,
			animeState = "walkleft",
			gLeft = 0,
			gRight = 1
		}
	elseif x == 3 then
		values = { 
			forceX = 11050.01,
			forceY = 0.0,
			animeState = "walkright",
			gLeft = 0,
			gRight = 1
		}
	elseif x == 4 then
		values = { 
			forceX = 0.0,
			forceY = 482000.5,
			animeState = "idle",
			gLeft = 1,
			gRight = 0
		}
	elseif x == 5 then
		values = { 
			forceX = 0.0,
			forceY = 10.5,
			animeState = "idle",
			gLeft = 1,
			gRight = 0
		}
	else
		error("invalid")
	end

	return values
end
