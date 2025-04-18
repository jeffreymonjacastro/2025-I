import {
  Carousel,
  CarouselContent,
  CarouselItem,
  CarouselNext,
  CarouselPrevious,
} from "@/components/ui/carousel"
import { Card, CardContent } from "@/components/ui/card"
import Image from 'next/image';

export default function Filter({ filter_type }: { filter_type: string }) {
  return (
    <>
      <Carousel
        opts={{
          align: "start",
        }}
        className="w-full max-w-[90%]"
      >
        <CarouselContent>
          {Array.from({ length: 12 }).map((_, index) => (
            <CarouselItem key={index} className="md:basis-1/2 lg:basis-1/3">
              <div className="p-1">
                <Card className="p-0">
                  <CardContent className="flex flex-col aspect-squre items-center justify-center p-0">
                    <p className="my-1">{`${index * 2 + 3} x ${index * 2 + 3}`}</p>
                    <Image
                      src={`./lenna/${filter_type}_${index * 2 + 3}.png`}
                      alt={`${filter_type} ${index * 2 + 3}`}
                      width={500}
                      height={500}
                      objectFit="cover"
                      className="rounded-lg shadow-lg"
                    />
                  </CardContent>
                </Card>
              </div>
            </CarouselItem>
          ))}
        </CarouselContent>
        <CarouselPrevious />
        <CarouselNext />
      </Carousel>
    </>
  );
}