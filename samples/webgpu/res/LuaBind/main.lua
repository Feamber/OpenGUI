local view = {}

local data = {value = 20}
function data:Add(args)
    print("lua event: " .. tostring(args:TryGet("eventName")))
    self.value = self.value + 1
    RouteEvent(view.slider, {element = view.slider})
end

function data:onValueChanged()
    print("lua value: " .. self.value)
    SendEventTo(view.cppDataModel, "Test", {element = view.slider})
end

function view:ReloadXml(ve)
    ve:SetPseudoClass(PseudoStates.Root, true)
    BindTree(ve, self.datamodel)
    self.slider = QueryFirst(ve, "#TestSlider")
    self.button = QueryFirst(ve, "#AddButton")
    self.datamodel:Bind(self.slider)
    self.datamodel:Bind(self.button)
    self.cppDataModel:Bind(self.slider)
end

function main(cppDataModel)
    view.datamodel=MakeDataModel(data, data)
    view.cppDataModel = cppDataModel
    LoadXml("res/LuaBind.xml", function(ve) view:ReloadXml(ve) end) 
end