print("Test Lua File Load.");

function TestGlobalFunc(n, s)
	if (n == 9999 and s == "TestString") then
		return 1;
	else
		return 0;
	end
end

