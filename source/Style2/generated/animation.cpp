//DO NOT MODIFY THIS FILE
//generated from Style2/mako/Struct.mako.cpp

#include <memory>
#include "OpenGUI/Style2/generated/animation.h"
#include "OpenGUI/Core/Utilities/string_hash.hpp"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/Style2/Rule.h"

void OGUI::AnimStyle::Initialize()
{
    animationName = {};
    animationDuration = 1.f;
    animationDelay = 0.f;
    animationDirection = EAnimDirection::Normal;
    animationIterationCount = 1.f;
    animationPlayState = {};
    animationFillMode = EAnimFillMode::Forwards;
    animationYieldMode = EAnimYieldMode::GoBack;
    animationResumeMode = EAnimResumeMode::Resume;
}

void OGUI::AnimStyle::ApplyProperties(const StyleSheetStorage& sheet, const gsl::span<StyleProperty>& props)
{
    for(auto& prop : props)
    {
        if(prop.keyword)
        {
            switch(prop.id)
            {
                case Id::animationName:
                    animationName = {};
                    break;
                case Id::animationDuration:
                    animationDuration = 1.f;
                    break;
                case Id::animationDelay:
                    animationDelay = 0.f;
                    break;
                case Id::animationDirection:
                    animationDirection = EAnimDirection::Normal;
                    break;
                case Id::animationIterationCount:
                    animationIterationCount = 1.f;
                    break;
                case Id::animationPlayState:
                    animationPlayState = {};
                    break;
                case Id::animationFillMode:
                    animationFillMode = EAnimFillMode::Forwards;
                    break;
                case Id::animationYieldMode:
                    animationYieldMode = EAnimYieldMode::GoBack;
                    break;
                case Id::animationResumeMode:
                    animationResumeMode = EAnimResumeMode::Resume;
                    break;
            }
        }
        else
        {
            switch(prop.id)
            {
                case Id::animationName:
                    animationName = sheet.Get<std::string>(prop.value);
                    break;
                case Id::animationDuration:
                    animationDuration = sheet.Get<float>(prop.value);
                    break;
                case Id::animationDelay:
                    animationDelay = sheet.Get<float>(prop.value);
                    break;
                case Id::animationDirection:
                    animationDirection = sheet.Get<EAnimDirection>(prop.value);
                    break;
                case Id::animationIterationCount:
                    animationIterationCount = sheet.Get<float>(prop.value);
                    break;
                case Id::animationPlayState:
                    animationPlayState = sheet.Get<AnimTimingFunction>(prop.value);
                    break;
                case Id::animationFillMode:
                    animationFillMode = sheet.Get<EAnimFillMode>(prop.value);
                    break;
                case Id::animationYieldMode:
                    animationYieldMode = sheet.Get<EAnimYieldMode>(prop.value);
                    break;
                case Id::animationResumeMode:
                    animationResumeMode = sheet.Get<EAnimResumeMode>(prop.value);
                    break;
            }
        }
    }
}


bool OGUI::AnimStyle::ParseProperties(StyleSheetStorage& sheet, std::string_view name, std::string_view value, StyleRule& rule, const char*& errorMsg, int animCount)
{
    size_t hash = OGUI::hash(name);

    //shorthands
    switch(hash)
    {
    }
    std::vector<std::string_view> tokens;
    std::split(value, tokens, ", ");
    //longhands
    switch(hash)
    {
        case Id::animationName:{
            if(tokens.size() > animCount)
            {
                errorMsg = "failed to parse animation-name value!(count dismatch)";
                return false;
            }
            for(int i=0; i<tokens.size(); ++i)
            {
                std::string v;
                if(ParseValue(tokens[i], v))
                    rule.animation[i].name = v;
                else
                {
                    errorMsg = "failed to parse animation-name value!";
                    return false;
                }
            }
            return true;
        }
        case Id::animationDuration:{
            if(tokens.size() > animCount)
            {
                errorMsg = "failed to parse animation-duration value!(count dismatch)";
                return false;
            }
            for(int i=0; i<tokens.size(); ++i)
            {
                float v;
                if(ParseValue(tokens[i], v))
                    rule.animation[i].properties.push_back({hash, sheet.Push(v), i});
                else
                {
                    errorMsg = "failed to parse animation-duration value!";
                    return false;
                }
            }
            return true;
        }
        case Id::animationDelay:{
            if(tokens.size() > animCount)
            {
                errorMsg = "failed to parse animation-delay value!(count dismatch)";
                return false;
            }
            for(int i=0; i<tokens.size(); ++i)
            {
                float v;
                if(ParseValue(tokens[i], v))
                    rule.animation[i].properties.push_back({hash, sheet.Push(v), i});
                else
                {
                    errorMsg = "failed to parse animation-delay value!";
                    return false;
                }
            }
            return true;
        }
        case Id::animationDirection:{
            if(tokens.size() > animCount)
            {
                errorMsg = "failed to parse animation-direction value!(count dismatch)";
                return false;
            }
            for(int i=0; i<tokens.size(); ++i)
            {
                EAnimDirection v;
                if(ParseValue(tokens[i], v))
                    rule.animation[i].properties.push_back({hash, sheet.Push(v), i});
                else
                {
                    errorMsg = "failed to parse animation-direction value!";
                    return false;
                }
            }
            return true;
        }
        case Id::animationIterationCount:{
            if(tokens.size() > animCount)
            {
                errorMsg = "failed to parse animation-iteration-count value!(count dismatch)";
                return false;
            }
            for(int i=0; i<tokens.size(); ++i)
            {
                float v;
                if(ParseValue(tokens[i], v))
                    rule.animation[i].properties.push_back({hash, sheet.Push(v), i});
                else
                {
                    errorMsg = "failed to parse animation-iteration-count value!";
                    return false;
                }
            }
            return true;
        }
        case Id::animationPlayState:{
            if(tokens.size() > animCount)
            {
                errorMsg = "failed to parse animation-play-state value!(count dismatch)";
                return false;
            }
            for(int i=0; i<tokens.size(); ++i)
            {
                AnimTimingFunction v;
                if(ParseValue(tokens[i], v))
                    rule.animation[i].properties.push_back({hash, sheet.Push(v), i});
                else
                {
                    errorMsg = "failed to parse animation-play-state value!";
                    return false;
                }
            }
            return true;
        }
        case Id::animationFillMode:{
            if(tokens.size() > animCount)
            {
                errorMsg = "failed to parse animation-fill-mode value!(count dismatch)";
                return false;
            }
            for(int i=0; i<tokens.size(); ++i)
            {
                EAnimFillMode v;
                if(ParseValue(tokens[i], v))
                    rule.animation[i].properties.push_back({hash, sheet.Push(v), i});
                else
                {
                    errorMsg = "failed to parse animation-fill-mode value!";
                    return false;
                }
            }
            return true;
        }
        case Id::animationYieldMode:{
            if(tokens.size() > animCount)
            {
                errorMsg = "failed to parse animation-yield-mode value!(count dismatch)";
                return false;
            }
            for(int i=0; i<tokens.size(); ++i)
            {
                EAnimYieldMode v;
                if(ParseValue(tokens[i], v))
                    rule.animation[i].properties.push_back({hash, sheet.Push(v), i});
                else
                {
                    errorMsg = "failed to parse animation-yield-mode value!";
                    return false;
                }
            }
            return true;
        }
        case Id::animationResumeMode:{
            if(tokens.size() > animCount)
            {
                errorMsg = "failed to parse animation-resume-mode value!(count dismatch)";
                return false;
            }
            for(int i=0; i<tokens.size(); ++i)
            {
                EAnimResumeMode v;
                if(ParseValue(tokens[i], v))
                    rule.animation[i].properties.push_back({hash, sheet.Push(v), i});
                else
                {
                    errorMsg = "failed to parse animation-resume-mode value!";
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}