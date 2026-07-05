#include <led/effects/effects.h>

#include <led/effects/solid.h>

void effect_apply(const struct effect *effect, struct strip *strip)
{
	switch (effect->params.kind) {
	case EFFECT_SOLID:
		effect_solid_apply(
			&effect->params.params.solid_params, effect->time, strip
		);
		break;
	}
}
