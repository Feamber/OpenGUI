local view = {value = 20}

function view:Add(args)
    print("lua event: " .. args:TryGet("element"):GetName())
    local x, y = args:TryGet("position")
    print("asd " .. tostring(x) .. ", " .. tostring(y))
    self.datamodel.value = self.datamodel.value + 1
    OGUI.RouteEvent(self.slider, {element = self.slider})
end

function view:SetValue(value)
    print("lua value: " .. self.datamodel.value)
    if self.slider then
        OGUI.SetStyleHeightPixel(self.button, self.datamodel.value + 20)
    end
    self.value = value
end

function view:GetValue()
    return self.value
end

function view:ReloadXml(ve)
    print(ve)
    ve:SetPseudoClass(OGUI.PseudoStates.Root, true)
    print(self.datamodel)
    OGUI.BindTree(ve, self.datamodel)
    self.slider = OGUI.QueryFirst(ve, "#TestSlider")
    self.button = OGUI.QueryFirst(ve, "#AddButton")
    self.text = OGUI.QueryFirst(ve, "#CountText")
    self.datamodel:Bind(self.slider)
    self.datamodel:Bind(self.button)
    self.cppDataModel:Bind(self.slider)
end

local data = 
{
    value = 
    {
        get = view.GetValue, 
        set = view.SetValue
    }
}

function main(cppDataModel)
    view.datamodel=OGUI.MakeDataModel(data, view)
    view.cppDataModel = cppDataModel
    LoadXml("res/LuaBind.xml", function(ve) view:ReloadXml(ve) end) 
end