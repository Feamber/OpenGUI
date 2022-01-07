local view = {}

local data = {value = 20}
function data:Add(args)
    print("lua event: " .. tostring(args:TryGet("element")))
    self.value = self.value + 1
    RouteEvent(view.slider, {element = view.slider})
end

function data:onValueChanged()
    print("lua value: " .. self.value)
    SendEventTo(view.cppDataModel, "Test", {element = view.slider})
end

function view:ReloadXml(ve)
    print(ve)
    ve:SetPseudoClass(PseudoStates.Root, true)
    print(ve)
    print(self.datamodel)
    BindTree(ve, self.datamodel)
    self.slider = QueryFirst(ve, "#TestSlider")
    self.button = QueryFirst(ve, "#AddButton")
    self.datamodel:Bind(self.slider)
    self.datamodel:Bind(self.button)
    self.cppDataModel:Bind(self.slider)
end

function main(cppDataModel)
    view.datamodel=MakeDataModel(data, data)
    print(view.datamodel)
    view.cppDataModel = cppDataModel
    print(cppDataModel)
    LoadXml("res/LuaBind.xml", function(ve) view:ReloadXml(ve) end) 
end