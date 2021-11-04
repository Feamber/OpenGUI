#include "OpenGUI/Style2/ComputedAnim.h"
#include "OpenGUI/Style2/Properties.h"
#include "OpenGUI/Style2/Rule.h"
#include "OpenGUI/Style2/ComputedStyle.h"

bool OGUI::ComputedAnim::Init(const gsl::span<StyleSheet*>& sheets)
{
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
		//todo
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

float OGUI::ComputedAnim::GetPercentage()
{
    float percentage = 0.f;
    float iteration = 0.f;
	float t = time - style.animationDelay;
	iteration = t / style.animationDuration;
	bool reversed = ShouldReverse(style.animationDirection, iteration);

    if (t <= 0)
	{
		if (test(style.animationFillMode, EAnimFillMode::Backwards))
		{
			//apply first frame
			bool reversed = style.animationDirection == EAnimDirection::Reverse || style.animationDirection == EAnimDirection::AlternateReverse;
			if (reversed)
				return 1.f;
			else
				return 0.f;
		}
	}
    else 
    {
        if (iteration > int(iteration))
            percentage = iteration - int(iteration);
        else
            percentage = iteration;
        percentage = reversed ? 1 - percentage : percentage;
        if (style.animationIterationCount > 0)
        {
            if (iteration >= style.animationIterationCount && !goingback)
            {
                if (test(style.animationFillMode, EAnimFillMode::Forwards))
                {
                    //apply last frame
                    if (reversed)
                        return 1.f;
                    else
                        return 0.f;
                }
            }
            iteration = std::clamp(iteration, 0.f, style.animationIterationCount);
        }
    }
    return percentage;
}

void OGUI::ComputedAnim::Apply(ComputedStyle &s)
{
    std::vector<AnimatedProperty> props;
	float percentage = GetPercentage();

    for(auto& track : tracks)
    {
        if(percentage == 0 && track.frames[0].percentage == 0)
            continue;
        int count = track.frames.size();
        int i=0;
        for(; i<count && track.frames[i].percentage < percentage; ++i);
        if(i == 0)
        {   
            if(track.frames[i].percentage == 0)
        }
        else 
        {
        
            
        }
    }
}