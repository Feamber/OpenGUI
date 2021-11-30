local data = {value = 20}
function data:Add()
    print("lua value: " .. self.value)
    self.value = self.value + 1
end

function data:onValueChanged()
    print("lua value: " .. self.value)
end

local function ReloadXml(ve, viewmodel)
    ve:SetPseudoClass(PseudoStates.Root, true)
    BindTree(ve, viewmodel)
    local slider = QueryFirst(ve, "#TestSlider")
    local button = QueryFirst(ve, "#AddButton")
    viewmodel:Bind(slider)
    viewmodel:Bind(button)
end

function main()
    local viewmodel=MakeDataModel(data);
    LoadXml("res/LuaBind.xml", function(ve) ReloadXml(ve, viewmodel) end)
end