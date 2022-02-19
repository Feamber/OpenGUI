#include "OpenGUI/Style2/AnimTypes.h"

#include "OpenGUI/Style2/ComputedAnim.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/ComputedStyle.h"

bool OGUI::ComputedAnim::Init(const gsl::span<StyleSheet*>& sheets)
{
    tracks.clear();
    StyleKeyframes* keyframes = nullptr;

    for (auto& s : sheets)
	{
		auto iter = s->namedKeyframes.find(style.animationName);
		if (iter != s->namedKeyframes.end())
		{
			sheet = s;
			keyframes = &s->styleKeyframes[iter->second];
			break;
		}
	}
    if(!keyframes)
        return false;
	int count = keyframes->keys.size();
    for(int i = 0; i < count; ++i)
    {
		auto& key = keyframes->keys[i];
		auto& fp = sheet->styleRules[key.frameIndex].properties;
        for(auto& prop : fp)
        {
            Track* track = nullptr;
            for(auto& t : tracks)
            {
                if(t.id == prop.id)
                {
                    track = &t;
                    break;
                }
            }
            if(!track)
            {
                tracks.push_back({});
                track = &tracks.back();
                track->id = prop.id;
            }
            
            Frame frame;
            frame.value = prop.value;
            frame.percentage = key.percentage;
            track->frames.push_back(frame);
        }
    }
    return true;
}


namespace OGUI
{
	float ApplyTimingFunction(AnimTimingFunction function, float percentage)
	{
		//TODO:
		return percentage;
	}
	bool ShouldReverse(EAnimDirection direction, float iteration)
	{
		int turn = int(iteration);

		if (direction == EAnimDirection::Reverse)
			return true;
		else if(direction == EAnimDirection::Alternate && turn % 2 == 1)
			return true;
		else if(direction == EAnimDirection::AlternateReverse && turn % 2 == 0)
			return true;
		else 
			return false;
	}
}

template<class T>
bool test(T a, T b)
{
	return ((int)a & (int)b) == (int)b;
}

bool OGUI::ComputedAnim::GetPercentage(float& percent)
{
    float iteration = 0.f;
	float t = time - style.animationDelay;
	iteration = t / style.animationDuration;
	bool reversed = ShouldReverse(style.animationDirection, iteration);

    if (t <= 0)
	{
		if (test(style.animationFillMode, EAnimFillMode::Backwards) || style.animationPlayState == EAnimPlayState::Paused)
		{
			//apply first frame
			bool reversed2 = style.animationDirection == EAnimDirection::Reverse || style.animationDirection == EAnimDirection::AlternateReverse;
			if (reversed2)
				percent = 1.f;
			else
				percent = 0.f;
            return true;
		}
        else
            return false;
	}
    else if (style.animationIterationCount > 0 && iteration >= style.animationIterationCount && !goingback)
    {
        if (test(style.animationFillMode, EAnimFillMode::Forwards) || style.animationPlayState == EAnimPlayState::Paused)
        {
            //apply last frame
            if (reversed)
                percent = 0.f;
            else
                percent = 1.f;
            return true;
        }
        else
            return false;
    }
    if (iteration > 1.f)
    {
        percent = iteration - int(iteration);
        if (percent == 0.f)
            percent = 1.f;
    }
    else
        percent = iteration;
    percent = reversed ? 1 - percent : percent;
    return true;
}

OGUI::RestyleDamage OGUI::ComputedAnim::Apply(ComputedStyle &s, const gsl::span<size_t>& override)
{
    std::vector<AnimatedProperty> props;
	float percentage;
    if(!GetPercentage(percentage))
        return RestyleDamage::None;

    for(auto& track : tracks)
    {
        if(percentage == 0 && track.frames[0].percentage == 0)
            continue;
        int count = track.frames.size();
        int i=0;
        for(; i<count && track.frames[i].percentage < percentage; ++i);
        if(i == 0)
        {   
            //TODO: per frame timing function
            auto alpha =  percentage / track.frames[i].percentage;
            alpha = ApplyTimingFunction(style.animationTimingFunction, alpha);
            props.push_back({track.id, track.frames[i].value, track.frames[i].value, alpha});
        }
        else if(i==count)
        {
            auto alpha = 0.f;
            props.push_back({track.id, track.frames[i-1].value, track.frames[i-1].value, alpha});
        }
        else 
        {
            auto alpha = (percentage - track.frames[i-1].percentage) / (track.frames[i].percentage - track.frames[i-1].percentage);
            alpha = ApplyTimingFunction(style.animationTimingFunction, alpha);
            props.push_back({track.id, track.frames[i-1].value, track.frames[i].value, alpha});
        }
    }

    return s.ApplyAnimatedProperties(sheet->storage, props, override);
}